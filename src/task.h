// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QTime>
#include <QJsonDocument>
#include <QJsonArray>
#include <ostream>
#include <memory>
#include <QJsonObject>

struct Task {
    long long projectId;
    long long taskId;
    unsigned int timeEntryId;
    QString clientName;
    QString projectName;
    QString taskName;
    QTime timeTracked;
    QString note;
    bool started{false};
    bool shouldAutomaticallyStop{false};
    QDate date{QDate::currentDate()};

    // data starting here won't be saved to streams
    bool favourited{false};

    static const QString values_separator;
    static const QString end_line_separator;

    friend QJsonArray &operator<<(QJsonArray& json, const std::shared_ptr<Task>& task) {
        QJsonObject newObject;
        newObject[PROJECT_ID_KEY] = task->projectId;
        newObject[TASK_ID_KEY] = task->taskId;
        newObject[TIME_ENTRY_ID_KEY] = static_cast<int>(task->timeEntryId);
        newObject[CLIENT_NAME_KEY] = task->clientName;
        newObject[PROJECT_NAME_KEY] = task->projectName;
        newObject[TASK_NAME_KEY] = task->taskName;
        newObject[TIME_TRACKED_KEY] = task->timeTracked.msecsSinceStartOfDay();
        newObject[NOTE_KEY] = task->note;
        newObject[STARTED_KEY] = task->started;
        newObject[DATE_KEY] = task->date.toJulianDay();

        json.append(newObject);
        return json;
    }

    friend const QJsonObject &operator>>(const QJsonObject &taskJson, std::shared_ptr<Task> &task) {
        task->projectId = taskJson[PROJECT_ID_KEY].toInt();
        task->taskId = taskJson[TASK_ID_KEY].toInt();
        task->timeEntryId = taskJson[TIME_ENTRY_ID_KEY].toInt();
        task->clientName = taskJson[CLIENT_NAME_KEY].toString();
        task->projectName = taskJson[PROJECT_NAME_KEY].toString();
        task->taskName = taskJson[TASK_NAME_KEY].toString();
        task->timeTracked = QTime::fromMSecsSinceStartOfDay(taskJson[TIME_TRACKED_KEY].toInt());
        task->note = taskJson[NOTE_KEY].toString();
        task->started = taskJson[STARTED_KEY].toBool();
        task->date = QDate::fromJulianDay(taskJson[DATE_KEY].toInt());

        return taskJson;
    }

    bool operator==(const Task &other_task) const {
        return projectId == other_task.projectId &&
               taskId == other_task.taskId &&
               timeEntryId == other_task.timeEntryId &&
               clientName == other_task.clientName &&
               projectName == other_task.projectName &&
               started == other_task.started &&
               taskName == other_task.taskName &&
               note == other_task.note;
    }

    bool operator!=(const Task &other_task) const {
        return !(other_task == *this);
    }

    [[nodiscard]] QString get_project_label() const {
        if (clientName.isNull() || clientName.isEmpty()) {
            return projectName;
        }

        return QString("%1 (%2)").arg(projectName, clientName);
    }

    void update_task_from_project_label(const QString &projectLabel) {
        QStringList labelComponents{projectLabel.split(" (")};
        this->projectName = labelComponents.at(0);
        if (labelComponents.size() == 2) {
            const QString &clientNameLabel = labelComponents.at(1);
            this->clientName = clientNameLabel.left(clientNameLabel.lastIndexOf(")"));
        }
    }

private:
    static const QString PROJECT_ID_KEY;
    static const QString TASK_ID_KEY;
    static const QString TIME_ENTRY_ID_KEY;
    static const QString CLIENT_NAME_KEY;
    static const QString PROJECT_NAME_KEY;
    static const QString TASK_NAME_KEY;
    static const QString TIME_TRACKED_KEY;
    static const QString NOTE_KEY;
    static const QString STARTED_KEY;
    static const QString DATE_KEY;
};

#endif // TASK_H
