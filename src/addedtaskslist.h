#ifndef ADDEDTASKSLIST_H
#define ADDEDTASKSLIST_H

#include <QVector>
#include <QMap>
#include <QDate>
#include <QObject>

#include "task.h"
#include "harvesthandler.h"

class AddedTasksList : public QObject {
Q_OBJECT

public:
    explicit AddedTasksList(QObject *parent = nullptr);

    [[nodiscard]] QVector<Task *> tasks() const;

    bool taskEdited(int index, const Task *task);

    void stopTask(int index);

    void startTask(int index);

signals:

    void preTaskAdded();

    void postTaskAdded();

    void preTaskRemoved(int index);

    void postTaskRemoved();

public slots:

    void taskAdded(Task *task);

    void taskRemoved(int index);

private:
    QMap<QDate, QVector<Task *>> mTasks;
    QDate current_date;

    HarvestHandler *const harvestHandler;
};

#endif // ADDEDTASKSLIST_H
