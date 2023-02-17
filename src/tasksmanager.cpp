// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

#include "tasksmanager.h"

TasksManager::TasksManager(QObject *parent,
                           ProjectsList &projectsList,
                           AddedTasksList &addedTaskList,
                           FavouritesList &favouritesList)
        : QObject(parent)
        , mProjects{projectsList}
        , mAddedTasks{addedTaskList}
        , mFavourites{favouritesList}
        , harvestHandler{HarvestHandler::instance()}
        , zero_time{QTime(0, 0)}
        , appDate{AppDate::instance()} {
    connect(harvestHandler.get(), &HarvestHandler::task_added, this, &TasksManager::tasksAdded);
    if (harvestHandler->is_ready()) {
        harvestHandler->list_tasks(appDate->date().addDays(-2), appDate->date().addDays(2));
    } else {
        connect(harvestHandler.get(), &HarvestHandler::ready, this, [this] {
            harvestHandler->list_tasks(appDate->date().addDays(-2), appDate->date().addDays(2));
        });
    }
}

ProjectsList &TasksManager::projectsList() const {
    return mProjects;
}

AddedTasksList &TasksManager::addedTasksList() const {
    return mAddedTasks;
}

FavouritesList &TasksManager::favouritesList() const {
    return mFavourites;
}

void TasksManager::tasksAdded(const TaskPointer& task) {
    // Flag task as favourite if it's in our favourites list
    lookupFavouritesFromTask(task, [task](QVector<TaskPtr>::const_iterator) {
        task->favourited = true;
    });

    mAddedTasks.taskAdded(task);
}

void TasksManager::newTaskAdded(int projectIndex, int taskIndex, const QString &note, const QString &time) {
    if (projectIndex < 0 || projectIndex >= mProjects.projects().size())
        return;

    const HarvestProject project{mProjects.projects().at(projectIndex)};
    if (taskIndex < 0 || taskIndex >= project.task.size())
        return;

    const HarvestTask task{project.task.at(taskIndex)};

    const QTime timeTracked{QTime::fromString(time, "HH:mm")};
    TaskPtr newTask{std::make_shared<Task>(Task{
            .projectId=project.projectId,
            .taskId=task.task_id,
            .timeEntryId=0,
            .clientName=task.client_name,
            .projectName=project.projectName,
            .taskName=task.task_name,
            .timeTracked=timeTracked,
            .note=note,
            .started=zero_time.secsTo(timeTracked) == 0,
            .date=appDate->date(),
            .favourited=mFavourites.isFavourited(project.projectId, task.task_id)
    })};
    harvestHandler->add_task(newTask);
}

void TasksManager::taskUpdated(const int index, int projectIndex, int taskIndex,
                               const QString &note, const QString &time) {
    if (projectIndex < 0 || projectIndex >= mProjects.projects().size())
        return;
    const HarvestProject project{mProjects.projects().at(projectIndex)};

    if (taskIndex < 0 || taskIndex >= project.task.size())
        return;
    const HarvestTask task{project.task.at(taskIndex)};

    const QTime timeTracked{QTime::fromString(time, "HH:mm")};
    TaskPtr editedTask{mAddedTasks.tasks().at(index)};
    editedTask->projectId = project.projectId;
    editedTask->taskId = task.task_id;
    editedTask->clientName = task.client_name;
    editedTask->projectName = project.projectName;
    editedTask->taskName = task.task_name;
    editedTask->timeTracked = timeTracked;
    editedTask->note = note;
    harvestHandler->update_task(editedTask);
}

void TasksManager::addFavouriteFromAddedTask(const int tasksIndex) const {
    const TaskPtr& task {mAddedTasks.tasks().at(tasksIndex)};

    mFavourites.favouriteAdded(task);
}

void TasksManager::removeFavouriteFromAddedTask(const int tasksIndex) const {
    const TaskPtr& addedTask{mAddedTasks.tasks().at(tasksIndex)};

    lookupFavouritesFromTask(addedTask, [this](QVector<TaskPtr>::const_iterator favouriteFound) {
        mFavourites.favouriteRemoved(favouriteFound);
    });
}

void TasksManager::lookupFavouritesFromTask(const TaskPtr& addedTask,
                                            const std::function<void(QVector<TaskPtr>::const_iterator)> &toDo) const {
    QVector<TaskPtr> addedFavouritesList{mFavourites.favourites()};
    QVector<TaskPtr>::const_iterator favouriteFound{
            std::find_if(addedFavouritesList.constBegin(), addedFavouritesList.constEnd(),
                         [addedTask](const TaskPtr& task) {
                             return addedTask->projectId == task->projectId && addedTask->taskId == task->taskId;
                         })
    };
    if(favouriteFound == addedFavouritesList.constEnd()) {
        return;
    }

    toDo(favouriteFound);
}
