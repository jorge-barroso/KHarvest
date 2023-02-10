#include "harvesthandler.h"
#include "harvesttask.h"
#include "harvestproject.h"
#include "task.h"
#include "kharvestconfig.h"

#include <QApplication>
#include <QTcpSocket>
#include <QDataStream>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QRegularExpression>
#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QDebug>

#include <cmath>

HarvestHandler *HarvestHandler::harvest_handler{nullptr};
const QString HarvestHandler::default_grant_type{"authorization_code"};
const QString HarvestHandler::refresh_grant_type{"refresh_token"};
const int HarvestHandler::request_timeout_constant{10 * 1000};

HarvestHandler *HarvestHandler::instance() {
    if (harvest_handler == nullptr) {
        harvest_handler = new HarvestHandler();
    }

    return harvest_handler;
}

HarvestHandler::HarvestHandler()
        : auth_server{nullptr}
        , auth_socket{nullptr}
        , network_manager(this)
        , loop()
        , is_network_reachable{true} {

    network_manager.setAutoDeleteReplies(true);

    // this auth is considered found not only if we read it, but we also need to make sure all the necessary fields are present
    auth_found = !KHarvestConfig::access_token().isEmpty() && json_auth_is_complete();

    if (!auth_found) {
        login();
    } else {
        emit ready();
    }
}

HarvestHandler::~HarvestHandler() {
    delete auth_server;
    delete auth_socket;

    // https://stackoverflow.com/questions/19157946/does-stdmap-destructor-call-key-destructors-as-well-as-value-destructors
}

// this is an additional check that validates whether there's at least one more day of validity in the auth
// token or we need to refresh it before any requests can be performed
void HarvestHandler::check_authenticate() {
    if (!json_auth_is_safely_active()) {
        QString refresh_token(KHarvestConfig::refresh_token());
        authenticate_request(nullptr, &refresh_token);
    }
}

void HarvestHandler::login() {
    QDesktopServices::openUrl(login_url);

    auth_server = new QTcpServer();

    if (!auth_server->listen(QHostAddress::Any, 23456)) {
        const QString error_string{
                QApplication::translate("HarvestHandler", "Error while waiting for Harvest authorization")};
        QMessageBox::information(nullptr, QApplication::translate("HarvestHandler", "Error"), error_string);
        QApplication::quit();
    }

    connect(auth_server, &QTcpServer::newConnection, this, &HarvestHandler::new_connection);
}

bool HarvestHandler::json_auth_is_complete() {
    bool contains_access_token{!KHarvestConfig::access_token().isEmpty()};
    bool contains_refresh_token{!KHarvestConfig::refresh_token().isEmpty()};
    bool contains_token_type{!KHarvestConfig::token_type().isEmpty()};
    bool contains_expires_in{KHarvestConfig::expires_in() != 0};

    return contains_access_token && contains_refresh_token && contains_token_type && contains_expires_in;
}

bool HarvestHandler::json_auth_is_safely_active() {
    auto expires_on{QDateTime::fromMSecsSinceEpoch(KHarvestConfig::expires_on())};

    // if there's less than a full day of time left before the token expires...
    return expires_on > QDateTime::currentDateTime().addDays(1);
}

void HarvestHandler::new_connection() {
    auth_socket = auth_server->nextPendingConnection();

    connect(auth_socket, &QTcpSocket::readyRead, this, &HarvestHandler::code_received);
}

void HarvestHandler::code_received() {
    if (auth_socket->canReadLine()) {
        QStringList tokens = QString(auth_socket->readLine()).split(QRegularExpression("[ \r\n][ \r\n]*"));

        const QString message_string{QApplication::translate("HarvestHandler",
                                                             "Authentication successful, you may now close this tab")};
        const QString final_message = get_http_message(message_string);
        auth_socket->write(final_message.toUtf8());
        auth_socket->flush();
        auth_socket->close();
        auth_server->close();
        auth_socket->deleteLater();
        auth_server->deleteLater();

        for (QString &token: tokens) {
            if (token.contains("?")) {
                std::map<QString, QString> query_map{parse_query_string(token)};
                if (!query_map.contains("code") || !query_map.contains("scope")) {
                    QString error_string{
                            QApplication::translate("HarvestHandler",
                                                    "The details received from Harvest did not contain the minimum details required")};
                    QMessageBox::information(nullptr, QApplication::translate("HarvestHandler", "Incomplete Details"),
                                             error_string);
                    QApplication::quit();
                }
                authenticate_request(&query_map["code"], nullptr);
                get_user_details(query_map["scope"]);
            }
        }
    }
}

std::map<QString, QString> HarvestHandler::parse_query_string(QString &query_string) {
    // remove the starting "/?" from the query string
    query_string.remove(0, 2);


    QStringList values{query_string.split("&")};
    std::map<QString, QString> query_map;
    for (QString &value: values) {
        QStringList detailed_value{value.split("=")};
        query_map.emplace(detailed_value[0], detailed_value[1]);
    }

    return query_map;
}

void HarvestHandler::authentication_received(const QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NetworkError::NoError) {
        const QString error_string{
                QApplication::translate("HarvestHandler", "Error while authenticating with Harvest services: ") +
                reply->errorString()};
        QMessageBox::information(nullptr, QApplication::translate("HarvestHandler", "Error authenticating"),
                                 error_string);
        QApplication::quit();
        return;
    }

    // reply is originally non-const so this cast should be safe while allowing to use it
    // as const in the previous error handling
    QJsonDocument json_auth{read_close_reply(const_cast<QNetworkReply *>(reply))};
    QJsonObject json_object{json_auth.object()};
    qint64 seconds{json_object["expires_in"].toInt()};
    json_object.insert("expires_on", QDateTime::currentDateTime().addSecs(seconds).toMSecsSinceEpoch());
    json_auth.setObject(json_object);

    save_authentication(json_auth);
}

QJsonDocument HarvestHandler::read_close_reply(QNetworkReply *reply) {
    QByteArray response_body{static_cast<QString>(reply->readAll()).toUtf8()};
    reply->deleteLater();
    return QJsonDocument::fromJson(response_body);
}

void HarvestHandler::save_authentication(const QJsonDocument &json_auth) {
    KHarvestConfig::setAccess_token(json_auth["access_token"].toString());
    KHarvestConfig::setRefresh_token(json_auth["refresh_token"].toString());
    KHarvestConfig::setExpires_in(json_auth["expires_in"].toVariant().toLongLong());
    KHarvestConfig::setExpires_on(json_auth["expires_on"].toVariant().toLongLong());
    KHarvestConfig::setToken_type(json_auth["token_type"].toString());
    KHarvestConfig::self()->save();
}

void HarvestHandler::authenticate_request(QString *auth_code, QString *refresh_token) {
    if (!is_network_reachable) {
        QMessageBox::warning(nullptr, QApplication::translate("HarvestHandler", "Network Unreachable"),
                             QApplication::translate("HarvestHandler",
                                                     "You are currently not connected to the internet, please reconnect and try again"));
        return;
    }

    QUrl request_url(auth_url);
    QNetworkRequest request(request_url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Harvest Timer Qt (jorge_barroso_11@hotmail.com)");
    request.setRawHeader("Accept", "application/json");

    QUrlQuery url_query;
    url_query.addQueryItem("client_id", client_id);
    url_query.addQueryItem("client_secret", client_secret);

    QString grant_type;
    if (auth_code != nullptr) {
        url_query.addQueryItem("code", *auth_code);
        grant_type = HarvestHandler::default_grant_type;
    } else if (refresh_token != nullptr) {
        url_query.addQueryItem("refresh_token", *refresh_token);
        grant_type = HarvestHandler::refresh_grant_type;
    }
    url_query.addQueryItem("grant_type", grant_type);


    QNetworkReply *reply{network_manager.post(request, url_query.toString().toUtf8())};

    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    authentication_received(reply);
}


QVector<HarvestProject> HarvestHandler::update_user_data() {
    if (!is_network_reachable) {
        QMessageBox::warning(nullptr,
                             QApplication::translate("HarvestHandler", "Network Unreachable"),
                             QApplication::translate("HarvestHandler",
                                                     "You are currently not connected to the internet, please reconnect and try again"));
        return projects;
    }

    QUrl request_url(assignments_url);

    int total_pages = -1;
    for (int page = 1; total_pages == -1 || page <= total_pages; ++page) {
        QUrlQuery url_query;
        url_query.addQueryItem("page", QString::number(page));
        url_query.addQueryItem("per_page", pagination_records);
        request_url.setQuery(url_query);

        QNetworkReply *reply{do_request_with_auth(request_url, true, "GET")};
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << reply->error();
        }

        const QJsonDocument json_payload{read_close_reply(reply)};
        get_projects_data(json_payload, projects);

        if (total_pages == -1) {
            total_pages = json_payload["total_pages"].toInt();
        }
    }

    return projects;
}

void
HarvestHandler::get_projects_data(const QJsonDocument &json_payload, QVector<HarvestProject> &projects_vector) {
    for (const QJsonValue project_assignment: json_payload["project_assignments"].toArray()) {
        const QString client_name{project_assignment["client"]["name"].toString()};
        QVector<HarvestTask> project_tasks_vector;
        for (const QJsonValue json_task: project_assignment["task_assignments"].toArray()) {
            const QJsonValue json_task_details{json_task["task"]};
            const HarvestTask task = HarvestTask{
                    .task_id = json_task_details["id"].toInt(),
                    .task_name = json_task_details["name"].toString(),
                    .client_name = client_name
            };

            project_tasks_vector.append(task);
        }

        const HarvestProject project{
                project_assignment["client"]["name"].toString(),
                project_assignment["project"]["name"].toString(),
                project_assignment["project"]["id"].toInt(),
                project_tasks_vector
        };
        projects_vector.append(project);
    }
}

void HarvestHandler::get_user_details(const QString &scope) {
    KHarvestConfig::setAccount_id(scope.split("%3A")[1]);
    KHarvestConfig::setUser_id(get_user_id());

    emit ready();
}

bool HarvestHandler::is_ready() const {
    return auth_found;
}

// This method preloads a list of tasks from the harvest system based on the dates provided
// It is used to preload our TaskScrollArea with the tasks the user had previously added
void HarvestHandler::list_tasks(const QDate &from_date, const QDate &to_date) {
    if (!is_network_reachable) {
        QMessageBox::warning(nullptr, QApplication::translate("HarvestHandler", "Network Unreachable"),
                             QApplication::translate("HarvestHandler",
                                                     "You are currently not connected to the internet, please reconnect and try again"));
        return;
    }

    QUrl request_url(time_entries_url);
    QUrlQuery url_query;
    url_query.addQueryItem("from", from_date.toString(Qt::ISODate));
    url_query.addQueryItem("to", to_date.toString(Qt::ISODate));
    url_query.addQueryItem("user_id", KHarvestConfig::user_id());
    request_url.setQuery(url_query);

    QNetworkReply *reply{do_request_with_auth(request_url, false, "GET")};
    connect(reply, &QNetworkReply::finished, this, &HarvestHandler::tasks_list_ready);
}

void HarvestHandler::add_task(Task *task) {
    if (!is_network_reachable) {
        QMessageBox::warning(nullptr, QApplication::translate("HarvestHandler", "Network Unreachable"),
                             QApplication::translate("HarvestHandler",
                                                     "You are currently not connected to the internet, please reconnect and try again"));
        return;
    }

    const QString spent_date{task->date.toString(Qt::ISODate)};
    const float seconds{static_cast<float>(QTime(0, 0).secsTo(task->timeTracked))};

    QJsonObject request_payload;
    request_payload.insert("project_id", task->projectId);
    request_payload.insert("task_id", task->taskId);
    request_payload.insert("spent_date", QJsonValue(spent_date));
    request_payload.insert("notes", task->note);
    request_payload.insert("hours", seconds / 60 / 60);
    request_payload.insert("is_running", seconds == 0);

    // We save our task in a map so that we can retrieve it later when the response comes
    size_t key{qHash(QString::number(task->projectId).append(QString::number(task->taskId)))};
    tasks_queue.insert({key, task});

    QNetworkReply *reply{do_request_with_auth(time_entries_url, false, "POST", QJsonDocument(request_payload))};
    connect(reply, &QNetworkReply::finished, this, &HarvestHandler::add_task_checks);
}

void HarvestHandler::update_task(const Task *updated_task) {
    if (!is_network_reachable) {
        QMessageBox::warning(nullptr, QApplication::translate("HarvestHandler", "Network Unreachable"),
                             QApplication::translate("HarvestHandler",
                                                     "You are currently not connected to the internet, please reconnect and try again"));
        return;
    }

    const float seconds{static_cast<float>(QTime(0, 0).secsTo(updated_task->timeTracked))};

    QJsonObject request_payload;
    request_payload.insert("projectId", updated_task->projectId);
    request_payload.insert("taskId", updated_task->taskId);
    request_payload.insert("notes", updated_task->note);
    request_payload.insert("hours", seconds / 60 / 60);

    // We save our task in a map so that we can retrieve it later when the response comes
    size_t key{qHash(QString::number(updated_task->projectId).append(QString::number(updated_task->taskId)))};
    tasks_queue.insert({key, const_cast<Task *>(updated_task)});

    QString update_url{time_entries_url + "/" + QString::number(updated_task->timeEntryId)};
    QNetworkReply *reply{do_request_with_auth(update_url, false, "PATCH", QJsonDocument(request_payload))};
    connect(reply, &QNetworkReply::finished, this, &HarvestHandler::update_task_checks);
}


void HarvestHandler::start_task(const Task &task) {
    if (!is_network_reachable) {
        QMessageBox::warning(nullptr, QApplication::translate("HarvestHandler", "Network Unreachable"),
                             QApplication::translate("HarvestHandler",
                                                     "You are currently not connected to the internet, please reconnect and try again"));
        return;
    }

    const QUrl url{time_entries_url + "/" + QString::number(task.timeEntryId) + "/restart"};
    QNetworkReply *reply{do_request_with_auth(url, false, "PATCH")};
    connect(reply, &QNetworkReply::finished, this, &HarvestHandler::start_task_checks);
}

void HarvestHandler::stop_task(const Task &task) {
    if (!is_network_reachable) {
        QMessageBox::warning(nullptr, QApplication::translate("HarvestHandler", "Network Unreachable"),
                             QApplication::translate("HarvestHandler",
                                                     "You are currently not connected to the internet, please reconnect and try again"));
        return;
    }

    const QUrl url{time_entries_url + "/" + QString::number(task.timeEntryId) + "/stop"};
    QNetworkReply *reply{do_request_with_auth(url, false, "PATCH")};
    connect(reply, &QNetworkReply::finished, this, &HarvestHandler::stop_task_checks);
}

void HarvestHandler::delete_task(const Task &task) {
    if (!is_network_reachable) {
        QMessageBox::warning(nullptr, QApplication::translate("HarvestHandler", "Network Unreachable"),
                             QApplication::translate("HarvestHandler",
                                                     "You are currently not connected to the internet, please reconnect and try again"));
        return;
    }

    const QUrl url{time_entries_url + "/" + QString::number(task.timeEntryId)};
    QNetworkReply *reply{do_request_with_auth(url, false, "DELETE")};
    connect(reply, &QNetworkReply::finished, this, &HarvestHandler::delete_task_checks);
}

QNetworkReply *HarvestHandler::do_request_with_auth(const QUrl &url, const bool sync_request, const QByteArray &verb,
                                                    const std::optional<QJsonDocument> &payload) {
    check_authenticate();

    QNetworkRequest request(url);
    request.setTransferTimeout(HarvestHandler::request_timeout_constant);
    request.setRawHeader("Authorization", "Bearer " + KHarvestConfig::access_token().toUtf8());
    request.setRawHeader("Harvest-Account-Id", KHarvestConfig::account_id().toUtf8());

    QNetworkReply *reply;
    if (payload.has_value()) {
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        reply = network_manager.sendCustomRequest(request, verb, payload->toJson(QJsonDocument::JsonFormat::Compact));
    } else {
        reply = network_manager.sendCustomRequest(request, verb, "");
    }

    if (sync_request) {
        // Execute the event loop here, now we will wait here until readyRead() signal is emitted
        // which in turn will trigger event loop quit.
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
    }

    return reply;
}

void HarvestHandler::tasks_list_ready() {
    QNetworkReply *reply{dynamic_cast<QNetworkReply *>(sender())};
    if (default_error_check(reply, QApplication::translate("HarvestHandler", "Error Loading Tasks"),
                            QApplication::translate("HarvestHandler", "Could not load your tasks: "))) {
        return;
    }

    // Get response from the reply object
    const QJsonDocument list_tasks_response{read_close_reply(reply)};
    const QJsonValue tasks_object{list_tasks_response.object().value("time_entries")};

    for (const QJsonValueRef &&harvestTask: tasks_object.toArray()) {
        const QJsonObject task_object{harvestTask.toObject()};
        const unsigned int task_entry_id{static_cast<unsigned int>(task_object["id"].toInt())};
        const long long int project_id{task_object["project"]["id"].toInt()};
        const QString project_name{task_object["project"]["name"].toString()};
        const QString client_name{task_object["client"]["name"].toString()};
        const long long int task_id{task_object["task"]["id"].toInt()};
        const QString task_name{task_object["task"]["name"].toString()};
        const QString note{task_object["notes"].toString()};

        const double time_tracked_decimal{task_object["hours"].toDouble()};
        double tracked_hours, tracked_minutes;
        tracked_minutes = std::modf(time_tracked_decimal, &tracked_hours);
        const QTime time_tracked(static_cast<int>(tracked_hours), static_cast<int>(tracked_minutes * 60));

        const bool started{!task_object["started_time"].toString().isNull()};
        const QDate task_date{QDate::fromString(task_object["spent_date"].toString(), Qt::DateFormat::ISODate)};

        Task *task = new Task{
                .projectId = project_id,
                .taskId = task_id,
                .timeEntryId = task_entry_id,
                .clientName = client_name,
                .projectName = project_name,
                .taskName = task_name,
                .timeTracked = time_tracked,
                .note = note,
                .started = started,
                .date = task_date,
        };
        emit task_added(task);
    }
}

//
void HarvestHandler::add_task_checks() {
    QNetworkReply *reply{dynamic_cast<QNetworkReply *>(sender())};
    if (default_error_check(reply, QApplication::translate("HarvestHandler", "Error Adding Task"),
                            QApplication::translate("HarvestHandler", "Could not add your task: "))) {
        return;
    }

    // Get response from the reply object
    const QJsonDocument add_task_response{read_close_reply(reply)};

    const long long int project_id{add_task_response["project"]["id"].toInt()};
    const long long int task_id{add_task_response["task"]["id"].toInt()};
    const size_t key{qHash(QString::number(project_id).append(QString::number(task_id)))};

    const auto task_element = tasks_queue.find(key);
    if (task_element == tasks_queue.end()) {
        return;
    }

    // if we could find a task, let's fetch it and remove it from the queue map
    Task *task = task_element->second;
    tasks_queue.erase(task_element);
    task->timeEntryId = add_task_response["id"].toInt();

    emit task_added(task);
}

void HarvestHandler::update_task_checks() {
    auto *reply{dynamic_cast<QNetworkReply *>(sender())};
    default_error_check(reply, QApplication::translate("HarvestHandler", "Error Updating Task"),
                        QApplication::translate("HarvestHandler", "Could not update this task: "));
    reply->deleteLater();
}

void HarvestHandler::start_task_checks() {
    auto *reply{dynamic_cast<QNetworkReply *>(sender())};
    default_error_check(reply, QApplication::translate("HarvestHandler", "Error Starting Task"),
                        QApplication::translate("HarvestHandler", "Could not start this task: "));
    reply->deleteLater();
}

void HarvestHandler::stop_task_checks() {
    auto *reply{dynamic_cast<QNetworkReply *>(sender())};
    default_error_check(reply, QApplication::translate("HarvestHandler", "Error Stopping Task"),
                        QApplication::translate("HarvestHandler", "Could not stop this task: "));
    reply->deleteLater();
}

void HarvestHandler::delete_task_checks() {
    auto *reply{dynamic_cast<QNetworkReply *>(sender())};
    default_error_check(reply, QApplication::translate("HarvestHandler", "Error Deleting Task"),
                        QApplication::translate("HarvestHandler", "Could not delete this task: "));
    reply->deleteLater();
}

bool HarvestHandler::default_error_check(QNetworkReply *reply, const QString &base_error_title,
                                         const QString &base_error_body) {
    if (reply->error() == QNetworkReply::NetworkError::TimeoutError) {
        const QString error_string{
                base_error_body + QApplication::translate("HarvestHandler", "the request timed out.")};
        QMessageBox::information(nullptr, base_error_title, error_string);

    }
    if (reply->error() != QNetworkReply::NetworkError::NoError) {
        const QJsonDocument error_report{read_close_reply(const_cast<QNetworkReply *>(reply))};
        qDebug() << "received error: " << reply->error();
        qDebug() << "error message: " << error_report;

        const QString error_string{(error_report["error"].isNull() ?
                                    error_report["error"] :
                                    error_report["message"]).toString()};
        QMessageBox::information(nullptr, base_error_title, error_string);
        return true;
    }
    return false;
}

QString HarvestHandler::get_http_message(const QString &message) {
    QString final_message;
    final_message.append("HTTP/1.0 200 OK\r\n");
    final_message.append("Content-Type: text/html; charset=utf-8\r\n\r\n");
    final_message.append("<html>");
    final_message.append("<body>");
    final_message.append(message);
    final_message.append("</body>");
    final_message.append("</html>");
    return final_message;
}

void HarvestHandler::logout_cleanup() {
}

QString HarvestHandler::get_user_id() {
    QUrl request_url(user_url);

    QNetworkReply *reply{do_request_with_auth(request_url, true, "GET")};
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->error();
    }

    const QJsonDocument json_payload{read_close_reply(reply)};
    return QString::number(json_payload["id"].toDouble(), 'f', 0);
}
