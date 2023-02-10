//
// Created by jorge on 09/02/23.
//

#include "tasksmanager.h"


TasksManager::TasksManager(QObject *parent, ProjectsList *projectsList, AddedTasksList *addedTaskList)
        : QObject(parent), mProjects{projectsList}, mAddedTasks{addedTaskList},
          harvestHandler{HarvestHandler::instance()}, currentDate{QDate::currentDate()}, zero_time{QTime(0, 0)} {}

ProjectsList *TasksManager::projectsList() const {
    return mProjects;
}

AddedTasksList *TasksManager::addedTasksList() const {
    return mAddedTasks;
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
            .projectId=project.project_id,
            .taskId=task.task_id,
            .timeEntryId=0,
            .clientName=task.client_name,
            .projectName=project.project_name,
            .taskName=task.task_name,
            .timeTracked=timeTracked,
            .note=note,
            .started=zero_time.secsTo(timeTracked) == 0,
            .date{currentDate}
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
    editedTask->projectId = project.project_id;
    editedTask->taskId = task.task_id;
    editedTask->clientName = task.client_name;
    editedTask->projectName = project.project_name;
    editedTask->taskName = task.task_name;
    editedTask->timeTracked = timeTracked;
    editedTask->note = note;
    harvestHandler->update_task(editedTask);
}

long TasksManager::projectIndexByName(const QString &projectName) {
    const QVector<HarvestProject> &harvestProjects{mProjects->projects()};
    const long index{std::distance(harvestProjects.begin(), std::find_if(harvestProjects.begin(), harvestProjects.end(),
                                                                         [&projectName](const HarvestProject &project) {
                                                                             return project.project_name == projectName;
                                                                         })
    )};
    return index;
}

long TasksManager::taskIndexByName(const QString &taskName) {
    const QVector<HarvestTask> &harvestTasks{mProjects->tasks()};
    const long index{std::distance(harvestTasks.begin(), std::find_if(harvestTasks.begin(), harvestTasks.end(),
                                                                      [taskName](const HarvestTask &task) {
                                                                          return task.task_name == taskName;
                                                                      })
    )};
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
