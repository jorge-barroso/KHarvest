#include "addedtaskslist.h"
#include "harvesthandler.h"
#include "maputils.h"

AddedTasksList::AddedTasksList(QObject *parent)
        : QObject{parent}, current_date{QDate::fromJulianDay(0)} {
    auto *harvest_handler{HarvestHandler::instance()};
    connect(harvest_handler, &HarvestHandler::task_added, this, &AddedTasksList::task_added);
    if (harvest_handler->is_ready()) {
        current_date = QDate::currentDate();
        harvest_handler->list_tasks(current_date.addDays(-2), current_date.addDays(2));
    } else {
        connect(harvest_handler, &HarvestHandler::ready, this, [this, harvest_handler] {
            harvest_handler->list_tasks(current_date.addDays(-2), current_date.addDays(2));
        });
    }
}

void AddedTasksList::task_added(Task *task) {
    // TODO favourite status
    emit preTaskAdded();

    MapUtils::map_insert_or_create_vector(mTasks, task->date, task);

    emit postTaskAdded();
}

QVector<Task *> AddedTasksList::tasks() const {
    QMap<QDate, QVector<Task *>>::const_iterator lb{mTasks.constFind(current_date)};
    if (lb == mTasks.constEnd()) {
        return {};
    }

    return lb.value();
}