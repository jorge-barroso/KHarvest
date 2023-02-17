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
    typedef std::shared_ptr<Task> TaskPtr;
public:
    explicit AddedTasksList(QObject *parent = nullptr);

    [[nodiscard]] QVector<TaskPtr> tasks() const;

    bool taskEdited(int index, const TaskPtr& task);

    void stopTask(int index);

    void startTask(int index);

signals:

    void preTaskAdded();

    void postTaskAdded();

    void preTaskRemoved(int index);

    void postTaskRemoved();

    void tasksDateChanged();

public slots:

    void appDateChanged();

    void taskAdded(const TaskPtr& task);

    void taskRemoved(int index);

private:
    QMap<QDate, QVector<TaskPtr>> mTasks;
    std::shared_ptr<AppDate> appDate;

    std::shared_ptr<HarvestHandler> harvestHandler;
};

#endif // ADDEDTASKSLIST_H
