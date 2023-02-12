#include "addedtaskslist.h"
#include "harvesthandler.h"
#include "maputils.h"

#include <QDebug>
AddedTasksList::AddedTasksList(AppDate *pDate, QObject *parent)
        : QObject{parent}
        , appDate{pDate}
        , harvestHandler{HarvestHandler::instance()}
{}

bool AddedTasksList::taskEdited(const int index, const Task *const task) {
    QMap<QDate, QVector<Task *>>::const_iterator lb{mTasks.constFind(appDate->date())};
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
    if (task->date == appDate->date())
            emit preTaskAdded();

    MapUtils::map_insert_or_create_vector(mTasks, task->date, task);

    if (task->date == appDate->date())
            emit postTaskAdded();
}

void AddedTasksList::taskRemoved(const int index) {
    QMap<QDate, QVector<Task *>>::iterator lb{mTasks.find(appDate->date())};
    if (lb == mTasks.end()) {
        return;
    }

    QVector<Task *> &tasks{lb.value()};
    if (index < 0 || index >= tasks.size()) {
        return;
    }

    emit preTaskRemoved(index);
    HarvestHandler::instance()->delete_task(*tasks.at(index));
    tasks.remove(index);
    emit postTaskRemoved();
}

QVector<Task *> AddedTasksList::tasks() const {
    QMap<QDate, QVector<Task *>>::const_iterator lb{mTasks.constFind(appDate->date())};
    if (lb == mTasks.constEnd()) {
        return {};
    }

    return lb.value();
}

void AddedTasksList::startTask(const int index) {
    Task *pTask = tasks().at(index);
    pTask->started = true;
    pTask->shouldAutomaticallyStop = false;
    harvestHandler->start_task(*pTask);
}

void AddedTasksList::stopTask(const int index) {
    Task *pTask = tasks().at(index);
    pTask->started = false;
    if (!pTask->shouldAutomaticallyStop) {
        harvestHandler->stop_task(*pTask);
    }
}

void AddedTasksList::appDateChanged() {
    emit tasksDateChanged();
}
