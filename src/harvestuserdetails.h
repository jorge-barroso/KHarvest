//
// Created by jorge on 09/02/23.
//

#ifndef KHARVEST_HARVESTUSERDETAILS_H
#define KHARVEST_HARVESTUSERDETAILS_H

#include <QString>
#include <ostream>
#include <KConfigGroup>

struct HarvestUserDetails {
    QString access_token;
    QString refresher_token;
    long expires_in;
    long long expires_on;
    QString token_type;

    friend KConfigGroup &operator<<(KConfigGroup &stream, const HarvestUserDetails &user_details) {
        stream << user_details.project_id << values_separator << user_details.task_id << values_separator << user_details.time_entry_id
               << values_separator << user_details.client_name << values_separator << user_details.project_name << values_separator
               << user_details.task_name << values_separator << user_details.time_tracked.toString() << values_separator << user_details.note
               << values_separator << user_details.started << values_separator
               << user_details.date.toString() << end_line_separator;

        return stream;
    }

    friend QTextStream &operator>>(QTextStream &stream, Task &task) {
        QString contents{stream.readLine()};
        QStringList values{contents.split(values_separator)};

        int i{-1};
        task.project_id = values[++i].toLongLong();
        task.task_id = values[++i].toLongLong();
        task.time_entry_id = values[++i].toLongLong();
        if (values.length() == 10) {
            task.client_name = values[++i];
        }
        task.project_name = values[++i];
        task.task_name = values[++i];
        task.time_tracked = QTime::fromString(values[++i]);
        task.note = values[++i];
        task.started = QVariant(values[++i]).toBool();
        task.date = QDate::fromString(values[++i]);

        return stream;
    }
};
#endif //KHARVEST_HARVESTUSERDETAILS_H
