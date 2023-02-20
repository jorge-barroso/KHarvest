#include "addedtaskslist.h"
#include "harvesthandler.h"
#include "maputils.h"

#include <QDebug>
AddedTasksList::AddedTasksList(QObject *parent)
        : QObject{parent}
        , appDate{AppDate::instance()}
        , harvestHandler{HarvestHandler::instance()}
{}

bool AddedTasksList::taskEdited(const int index, const TaskPtrRef& task) {
    QMap<QDate, QVector<TaskPtrRef>>::const_iterator lb{mTasks.constFind(appDate->date())};
    if (lb == mTasks.constEnd()) {
        return false;
    }

    QVector<TaskPtrRef> tasks{lb.value()};
    if (index < 0 || index >= tasks.size()) {
        return false;
    }

    TaskPtrRef old_task{tasks.value(index)};
    if (*old_task == *task) {
        return false;
    }

    tasks[index] = task;
    return true;
}

void AddedTasksList::taskAdded(const TaskPtrRef& task) {
    // TODO favourite status
    if (task->date == appDate->date())
            emit preTaskAdded();

    MapUtils::map_insert_or_create_vector(mTasks, task->date, task);

    if (task->date == appDate->date())
            emit postTaskAdded();
}

void AddedTasksList::taskRemoved(const int index) {
    QMap<QDate, QVector<TaskPtrRef>>::iterator lb{mTasks.find(appDate->date())};
    if (lb == mTasks.end()) {
        return;
    }

    QVector<TaskPtrRef> &tasks{lb.value()};
    if (index < 0 || index >= tasks.size()) {
        return;
    }

    emit preTaskRemoved(index);
    HarvestHandler::instance()->delete_task(*tasks.at(index));
    tasks.remove(index);
    emit postTaskRemoved();
}

QVector<AddedTasksList::TaskPtrRef> AddedTasksList::tasks() const {
    QMap<QDate, QVector<TaskPtrRef>>::const_iterator lb{mTasks.constFind(appDate->date())};
    if (lb == mTasks.constEnd()) {
        return {};
    }

    return lb.value();
}

void AddedTasksList::startTask(const int index) {
    const TaskPtrRef & pTask = tasks().at(index);
    pTask->started = true;
    pTask->shouldAutomaticallyStop = false;
    harvestHandler->start_task(*pTask);
}

void AddedTasksList::stopTask(const int index) {
    const TaskPtrRef & pTask = tasks().at(index);
    pTask->started = false;
    if (!pTask->shouldAutomaticallyStop) {
        harvestHandler->stop_task(*pTask);
    }
}

void AddedTasksList::appDateChanged() {
    emit tasksDateChanged();
}

void AddedTasksList::unfavouritedTask(const TaskPtrRef &unfavouritedTask) {
    for(const auto& tasksVector : mTasks) {
        for(auto& task : tasksVector) {
            if(task->projectId == unfavouritedTask->projectId && task->taskId == unfavouritedTask->taskId) {
                task->favourited = false;
            }
        }
    }
    emit favouritesChanged();
}
