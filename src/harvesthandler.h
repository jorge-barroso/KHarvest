// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

#ifndef HARVESTHANDLER_H
#define HARVESTHANDLER_H

#include <QObject>
#include <QString>
#include <QDir>
#include <QFile>
#include <QTcpServer>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QEventLoop>

#include <optional>
#include <KConfigGroup>

#include "harvestproject.h"
#include "task.h"
#include "kharvestconfig.h"
#include "keychain.h"

class HarvestHandler : public QObject {
Q_OBJECT
    Q_PROPERTY(bool isReady READ is_ready NOTIFY ready)
    Q_PROPERTY(bool error NOTIFY harvestError)
    Q_PROPERTY(bool warning NOTIFY harvestWarning)
    Q_PROPERTY(bool isOnline READ isOnline NOTIFY isOnlineChanged)

    typedef std::shared_ptr<Task> TaskPtr;
    typedef std::shared_ptr<HarvestHandler> HandlerPointer;
public slots:

    void new_connection();

    void logout_cleanup();

public:
    // We're taking a singleton approach here so the constructor will remain protected
    static HandlerPointer instance();

    QVector<HarvestProject> update_user_data();

    [[nodiscard]] bool is_ready() const;

    void add_task(TaskPtr &task);

    void update_task(TaskPtr &updatedTask);

    void start_task(const Task &task);

    void stop_task(const Task &task);

    void delete_task(const Task &task);

    void list_tasks(const QDate &from_date, const QDate &to_date);

    bool isOnline() const;
signals:

    void ready();

    void harvestError(const QString& errorMessage) const;

    void harvestWarning(const QString& warningMessage) const;

    void task_added(TaskPtr);

    void isOnlineChanged(bool isOnline) const;

public slots:
    void login();

private slots:

    void code_received();

    void authentication_received(QNetworkReply* reply);

    void tasks_list_ready();

    void update_task_checks();

    void add_task_checks();

    void start_task_checks();

    void stop_task_checks();

    void delete_task_checks();

private:
    HarvestHandler(); // Prevent construction

    static std::shared_ptr<HarvestHandler> harvest_handler;

    KeyChain keyChain;
    QString accessToken;
    QString refreshToken;
    qlonglong expiresIn{0};
    qlonglong expiresOn{0};
    QString tokenType;
    QString accountId;
    QString userId;

    static const QString AccessTokenKey;
    static const QString RefreshTokenKey;
    static const QString ExpiresInKey;
    static const QString ExpiresOnKey;
    static const QString TokenTypeKey;
    static const QString AccountIdKey;
    static const QString UserIdKey;

    std::shared_ptr<QTcpServer> auth_server;
    std::shared_ptr<QTcpSocket> auth_socket;

    const QString client_id{"VkKA3WoB2M5cEQGwf82VkeHb"};
    const QString client_secret{
            "QUwB8dtQxMwY5omBHgZBsXAhB2h_jzKZcGZkCUom1CPBYvTKUGPty7ree7ao92PV5FT5VQHbVWwNzTQUITVLmg"};
    static const QString default_grant_type;
    static const QString refresh_grant_type;

    // Authentication endpoints
    const QString auth_host{"https://id.getharvest.com"};
    const QString login_url{auth_host + "/oauth2/authorize?client_id=" + client_id + "&response_type=code"};
    const QString auth_url{auth_host + "/api/v2/oauth2/token"};

    // Task-related endpoints
    const QString requests_host{"https://api.harvestapp.com"};
    const QString assignments_url{requests_host + "/v2/users/me/project_assignments"};
    const QString time_entries_url{requests_host + "/v2/time_entries"};
    const QString user_url{requests_host + "/v2/users/me"};

    QVector<HarvestProject> projects;

    QNetworkAccessManager network_manager;

    bool auth_found;

    QEventLoop loop;

    const QString pagination_records{"100"};

    bool json_auth_is_complete();

    [[maybe_unused]]  bool json_auth_is_safely_active() const;

    static std::map<QString, QString> parse_query_string(QString &query_string);

    void getAuthDetails(const QJsonDocument &json_auth);

    void authenticate_request(const std::optional<QString> &auth_code, const std::optional<QString> &refresh_token);

    void getUserDetails(const QString &scope);

    QNetworkReply*  do_request_with_auth(const QUrl &url, bool sync_request, const QByteArray &verb,
                                                        const std::optional<QJsonDocument> &payload = std::nullopt);

    static void get_projects_data(const QJsonDocument &json_payload, QVector<HarvestProject> &projects_vector);

    static QJsonDocument read_close_reply(QNetworkReply* reply);

    bool default_error_check(QNetworkReply* reply, const QString &errorHeader,
                                    const QString &baseErrorBody);

    void check_authenticate();

    static QString get_http_message(const QString &message);

    std::unordered_map<size_t, TaskPtr> tasksQueue;

    static const int request_timeout_constant;
    bool mIsOnline;

    QString get_user_id();

    void saveData(bool includeAccount);

    static QString buildErrorMessage(const QString& header, const QString& body);
};

#endif // HARVESTHANDLER_H
