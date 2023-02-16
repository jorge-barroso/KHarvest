// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QTime>
#include <QTextStream>
#include <ostream>
#include <memory>

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

    friend QStringList &operator<<(QStringList &stream, const std::shared_ptr<Task>& task) {
        QString str{QString::number(task->projectId) + values_separator
                    + QString::number(task->taskId) + values_separator
                    + QString::number(task->timeEntryId) + values_separator
                    + task->clientName + values_separator
                    + task->projectName + values_separator
                    + task->taskName + values_separator
                    + task->timeTracked.toString() + values_separator
                    + task->note + values_separator
                    + QVariant(task->started).toString() + values_separator
                    + task->date.toString()};

        stream.append(str);
        return stream;
    }

    friend const QString &operator>>(const QString &taskLine, std::shared_ptr<Task> &task) {
        QStringList values{taskLine.split(values_separator)};

        int i{-1};
        task->projectId = values[++i].toLongLong();
        task->taskId = values[++i].toLongLong();
        task->timeEntryId = values[++i].toLongLong();
        task->clientName = values[++i];
        task->projectName = values[++i];
        task->taskName = values[++i];
        task->timeTracked = QTime::fromString(values[++i]);
        task->note = values[++i];
        task->started = QVariant(values[++i]).toBool();
        task->date = QDate::fromString(values[++i]);

        return taskLine;
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
};

#endif // TASK_H
