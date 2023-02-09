//
// Created by jorge on 09/02/23.
//

#include "tasksmanager.h"

#include <utility>


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
    qDebug() << zero_time;
    qDebug() << timeTracked;
    Task *newTask{new Task{
            .project_id=project.project_id,
            .task_id=task.task_id,
            .client_name=task.client_name,
            .project_name=project.project_name,
            .task_name=task.task_name,
            .time_tracked=timeTracked,
            .note=note,
            .started=zero_time.secsTo(timeTracked) == 0,
            .date{currentDate}
    }};
    harvestHandler->add_task(newTask);
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
