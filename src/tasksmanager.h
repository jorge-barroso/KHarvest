//
// Created by jorge on 09/02/23.
//

#ifndef KHARVEST_TASKSMANAGER_H
#define KHARVEST_TASKSMANAGER_H


#include <QObject>
#include "projectslist.h"
#include "addedtaskslist.h"

class TasksManager : public QObject {
Q_OBJECT
    Q_PROPERTY(ProjectsList *projectsList READ projectsList)
    Q_PROPERTY(AddedTasksList *tasksList READ addedTasksList)

public:
    TasksManager(QObject *parent, ProjectsList *projectsList, AddedTasksList *addedTaskList);

    [[nodiscard]] ProjectsList *projectsList() const;

    [[nodiscard]] AddedTasksList *addedTasksList() const;

public slots:

    void newTaskAdded(int projectIndex, int taskIndex, const QString &note, const QString &time);

    void taskUpdated(int index, unsigned int entryId, int projectIndex, int taskIndex, const QString &note,
                     const QString &time);

    long projectIndexByName(const QString &projectName);

    long taskIndexByName(const QString &taskName);

    void timeForward();

    void timeBackwards();

private:
    ProjectsList *mProjects;
    AddedTasksList *mAddedTasks;
    HarvestHandler *harvestHandler;
    QDate currentDate;
    QTime zero_time;

    void updateCurrentTime(int days);

};


#endif //KHARVEST_TASKSMANAGER_H
