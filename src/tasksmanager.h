//
// Created by jorge on 09/02/23.
//

#ifndef KHARVEST_TASKSMANAGER_H
#define KHARVEST_TASKSMANAGER_H


#include <QObject>
#include "projectslist.h"
#include "addedtaskslist.h"
#include "favouriteslist.h"

class TasksManager : public QObject {
Q_OBJECT

public:
    TasksManager(QObject *parent,
                 ProjectsList *projectsList,
                 AddedTasksList *addedTaskList,
                 FavouritesList *favouritesList);

    [[nodiscard]] ProjectsList *projectsList() const;

    [[nodiscard]] AddedTasksList *addedTasksList() const;

    [[nodiscard]] FavouritesList *favouritesList() const;

public slots:

    void newTaskAdded(int projectIndex, int taskIndex, const QString &note, const QString &time);

    void taskUpdated(int index, int projectIndex, int taskIndex, const QString &note, const QString &time);

    long projectIndexByName(const QString &projectLabel);

    long taskIndexByName(const QString &taskName);

    long projectIndexByHarvestId(qlonglong projectId);

    long taskIndexByHarvestId(qlonglong taskId);

    void addFavouriteFromAddedTask(int tasksIndex) const;

    void removeFavouriteFromAddedTask(int tasksIndex) const;

    void timeForward();

    void timeBackwards();

private:
    ProjectsList *mProjects;
    AddedTasksList *mAddedTasks;
    FavouritesList *mFavourites;
    HarvestHandler *harvestHandler;
    QDate currentDate;
    QTime zero_time;

    void updateCurrentTime(int days);

    void tasksAdded(Task *task);

    void lookupFavouritesFromTask(const Task *addedTask,
                                  const std::function<void(QVector<Task *>::const_iterator)> &toDo) const;

};


#endif //KHARVEST_TASKSMANAGER_H
