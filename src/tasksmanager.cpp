//
// Created by jorge on 09/02/23.
//

#include "tasksmanager.h"

TasksManager::TasksManager(QObject *parent,
                           ProjectsList *projectsList,
                           AddedTasksList *addedTaskList,
                           FavouritesList *favouritesList)
        : QObject(parent)
        , mProjects{projectsList}
        , mAddedTasks{addedTaskList}
        , mFavourites{favouritesList}
        , harvestHandler{HarvestHandler::instance()}
        , currentDate{QDate::currentDate()}
        , zero_time{QTime(0, 0)} {
    QObject::connect(harvestHandler, &HarvestHandler::task_added, this, &TasksManager::tasksAdded);
    if (harvestHandler->is_ready()) {
        harvestHandler->list_tasks(currentDate.addDays(-2), currentDate.addDays(2));
    } else {
        connect(harvestHandler, &HarvestHandler::ready, this, [this] {
            harvestHandler->list_tasks(currentDate.addDays(-2), currentDate.addDays(2));
        });
    }
}

ProjectsList *TasksManager::projectsList() const {
    return mProjects;
}

AddedTasksList *TasksManager::addedTasksList() const {
    return mAddedTasks;
}

FavouritesList *TasksManager::favouritesList() const {
    return mFavourites;
}

void TasksManager::tasksAdded(Task* task) {
    // Flag task as favourite if it's in our favourites list
    lookupFavouritesFromTask(task, [task](QVector<Task *>::const_iterator) {
        task->favourited = true;
    });

    mAddedTasks->taskAdded(task);
}

void TasksManager::newTaskAdded(int projectIndex, int taskIndex, const QString &note, const QString &time) {
    if (projectIndex < 0 || projectIndex >= mProjects->projects().size())
        return;

    const HarvestProject project{mProjects->projects().at(projectIndex)};
    if (taskIndex < 0 || taskIndex >= project.task.size())
        return;

    const HarvestTask task{project.task.at(taskIndex)};

    const QTime timeTracked{QTime::fromString(time, "HH:mm")};
    Task *newTask{new Task{
            .projectId=project.projectId,
            .taskId=task.task_id,
            .timeEntryId=0,
            .clientName=task.client_name,
            .projectName=project.projectName,
            .taskName=task.task_name,
            .timeTracked=timeTracked,
            .note=note,
            .started=zero_time.secsTo(timeTracked) == 0,
            .date=currentDate,
            .favourited=mFavourites->isFavourited(project.projectId, task.task_id)
    }};
    harvestHandler->add_task(newTask);
}

void TasksManager::taskUpdated(const int index, int projectIndex, int taskIndex,
                               const QString &note, const QString &time) {
    if (projectIndex < 0 || projectIndex >= mProjects->projects().size())
        return;
    const HarvestProject project{mProjects->projects().at(projectIndex)};

    if (taskIndex < 0 || taskIndex >= project.task.size())
        return;
    const HarvestTask task{project.task.at(taskIndex)};

    const QTime timeTracked{QTime::fromString(time, "HH:mm")};
    Task *editedTask{mAddedTasks->tasks().at(index)};
    editedTask->projectId = project.projectId;
    editedTask->taskId = task.task_id;
    editedTask->clientName = task.client_name;
    editedTask->projectName = project.projectName;
    editedTask->taskName = task.task_name;
    editedTask->timeTracked = timeTracked;
    editedTask->note = note;
    harvestHandler->update_task(editedTask);
}

long TasksManager::projectIndexByName(const QString &projectLabel) {
    const QVector<HarvestProject> &harvestProjects{mProjects->projects()};
    const HarvestProject *project = std::find_if(harvestProjects.begin(), harvestProjects.end(),
                                                 [&projectLabel](const HarvestProject &project) {
                                                     return project.get_project_label() == projectLabel;
                                                 });
    const long index{std::distance(harvestProjects.begin(), project)};

    return index;
}

long TasksManager::taskIndexByName(const QString &taskName) {
    const QVector<HarvestTask> &harvestTasks{mProjects->tasks()};
    const HarvestTask *task = std::find_if(harvestTasks.begin(), harvestTasks.end(),
                                           [taskName](const HarvestTask &task) {
                                               return task.task_name == taskName;
                                           });
    const long index{std::distance(harvestTasks.begin(), task)};
    return index;
}

long TasksManager::projectIndexByHarvestId(const qlonglong projectId) {
    const QVector<HarvestProject> &harvestProjects{mProjects->projects()};
    const HarvestProject *project = std::find_if(harvestProjects.begin(), harvestProjects.end(),
                                                 [projectId](const HarvestProject &project) {
                                                     return project.projectId == projectId;
                                                 });
    const long index{std::distance(harvestProjects.begin(), project)};

    return index;
}

long TasksManager::taskIndexByHarvestId(const qlonglong taskId) {
    const QVector<HarvestTask> &harvestTasks{mProjects->tasks()};
    const HarvestTask *task = std::find_if(harvestTasks.begin(), harvestTasks.end(),
                                           [taskId](const HarvestTask &task) {
                                               return task.task_id == taskId;
                                           });
    const long index{std::distance(harvestTasks.begin(), task)};
    return index;
}

void TasksManager::timeForward() {
    updateCurrentTime(1);
}

void TasksManager::timeBackwards() {
    updateCurrentTime(-1);
}

void TasksManager::updateCurrentTime(int days) {
    currentDate = currentDate.addDays(days);
    //TODO update projectsList
}

void TasksManager::addFavouriteFromAddedTask(const int tasksIndex) const {
    Task *task{mAddedTasks->tasks().at(tasksIndex)};

    mFavourites->favouriteAdded(task);
}

void TasksManager::removeFavouriteFromAddedTask(const int tasksIndex) const {
    Task *addedTask{mAddedTasks->tasks().at(tasksIndex)};

    lookupFavouritesFromTask(addedTask, [this](QVector<Task *>::const_iterator favouriteFound) {
        mFavourites->favouriteRemoved(favouriteFound);
    });
}

void TasksManager::lookupFavouritesFromTask(const Task *addedTask,
                                            const std::function<void(QVector<Task *>::const_iterator)> &toDo) const {
    QVector<Task *> addedFavouritesList{mFavourites->favourites()};
    QVector<Task *>::const_iterator favouriteFound{
            std::find_if(addedFavouritesList.constBegin(), addedFavouritesList.constEnd(),
                         [addedTask](const Task *task) {
                             return addedTask->projectId == task->projectId && addedTask->taskId == task->taskId;
                         })
    };
    if(favouriteFound == addedFavouritesList.constEnd()) {
        return;
    }

    toDo(favouriteFound);
}
