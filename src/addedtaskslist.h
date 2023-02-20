#ifndef ADDEDTASKSLIST_H
#define ADDEDTASKSLIST_H

#include <QVector>
#include <QMap>
#include <QDate>
#include <QObject>

#include <memory>

#include "task.h"
#include "harvesthandler.h"
#include "appdate.h"

class AddedTasksList : public QObject {
Q_OBJECT
    typedef std::shared_ptr<Task> TaskPtrRef;
public:
    explicit AddedTasksList(QObject *parent = nullptr);

    [[nodiscard]] QVector<TaskPtrRef> tasks() const;

    bool taskEdited(int index, const TaskPtrRef &task);

    void stopTask(int index);

    void startTask(int index);

    void unfavouritedTask(const TaskPtrRef &unfavouritedTask);

signals:

    void preTaskAdded();

    void postTaskAdded();

    void preTaskRemoved(int index);

    void postTaskRemoved();

    void tasksDateChanged();

    void favouritesChanged();

    void tasksReloaded();

public slots:

    void appDateChanged();

    void taskAdded(const TaskPtrRef &task);

    void taskRemoved(int index);

    void reload();

private:
    QMap<QDate, QVector<TaskPtrRef>> mTasks;
    QMap<QDate, QSet<qlonglong>> mTasksCache;
    std::shared_ptr<AppDate> appDate;

    std::shared_ptr<HarvestHandler> harvestHandler;
};

#endif // ADDEDTASKSLIST_H
