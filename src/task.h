#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QTime>
#include <QTextStream>
#include <ostream>

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
    QDate date{QDate::currentDate()};

    // data starting here won't be saved to streams
    bool favourited{false};

    static const QString values_separator;
    static const QString end_line_separator;

    friend QTextStream &operator<<(QTextStream &stream, const Task &task) {
        stream << task.projectId << values_separator << task.taskId << values_separator << task.timeEntryId
               << values_separator << task.clientName << values_separator << task.projectName << values_separator
               << task.taskName << values_separator << task.timeTracked.toString() << values_separator << task.note
               << values_separator << task.started << values_separator
               << task.date.toString() << end_line_separator;

        return stream;
    }

    friend QTextStream &operator>>(QTextStream &stream, Task &task) {
        QString contents{stream.readLine()};
        QStringList values{contents.split(values_separator)};

        int i{-1};
        task.projectId = values[++i].toLongLong();
        task.taskId = values[++i].toLongLong();
        task.timeEntryId = values[++i].toLongLong();
        task.clientName = values[++i];
        task.projectName = values[++i];
        task.taskName = values[++i];
        task.timeTracked = QTime::fromString(values[++i]);
        task.note = values[++i];
        task.started = QVariant(values[++i]).toBool();
        task.date = QDate::fromString(values[++i]);

        return stream;
    }

    bool operator==(const Task &other_task) const {
        return projectId == other_task.projectId &&
               taskId == other_task.taskId &&
               timeEntryId == other_task.timeEntryId &&
               clientName == other_task.clientName &&
               projectName == other_task.projectName &&
               taskName == other_task.taskName;
    }

    bool operator!=(const Task &other_task) const {
        return !(other_task == *this);
    }

    [[nodiscard]] QString get_project_label() const {
        if (clientName.isNull() || clientName.isEmpty()) {
            return projectName;
        }

        return projectName + " (" + clientName + ")";
    }
};

#endif // TASK_H
