#include "addedtaskslist.h"
#include "harvesthandler.h"
#include "maputils.h"

AddedTasksList::AddedTasksList(QObject *parent)
        : QObject{parent}, current_date{QDate::fromJulianDay(0)}, harvestHandler{HarvestHandler::instance()} {
    connect(harvestHandler, &HarvestHandler::task_added, this, &AddedTasksList::taskAdded);
    if (harvestHandler->is_ready()) {
        current_date = QDate::currentDate();
        harvestHandler->list_tasks(current_date.addDays(-2), current_date.addDays(2));
    } else {
        connect(harvestHandler, &HarvestHandler::ready, this, [this] {
            harvestHandler->list_tasks(current_date.addDays(-2), current_date.addDays(2));
        });
    }
}

bool AddedTasksList::taskEdited(const int index, const Task *const task) {
    QMap<QDate, QVector<Task *>>::const_iterator lb{mTasks.constFind(current_date)};
    if (lb == mTasks.constEnd()) {
        return false;
    }

    QVector<Task *> tasks{lb.value()};
    if (index < 0 || index >= tasks.size()) {
        return false;
    }

    const Task *old_task{tasks.value(index)};
    if (*old_task == *task) {
        return false;
    }

    tasks[index] = const_cast<Task *>(task);
    return true;
}

void AddedTasksList::taskAdded(Task *task) {
    // TODO favourite status
    emit preTaskAdded();

    MapUtils::map_insert_or_create_vector(mTasks, task->date, task);

    emit postTaskAdded();
}

void AddedTasksList::taskRemoved(const int index) {
    QMap<QDate, QVector<Task *>>::iterator lb{mTasks.find(current_date)};
    if (lb == mTasks.end()) {
        return;
    }

    QVector<Task *> &tasks{lb.value()};
    if (index < 0 || index >= tasks.size()) {
        return;
    }

    HarvestHandler::instance()->delete_task(*tasks.at(index));
    emit preTaskRemoved(index);
    tasks.remove(index);
    emit postTaskRemoved();
}

QVector<Task *> AddedTasksList::tasks() const {
    QMap<QDate, QVector<Task *>>::const_iterator lb{mTasks.constFind(current_date)};
    if (lb == mTasks.constEnd()) {
        return {};
    }

    return lb.value();
}