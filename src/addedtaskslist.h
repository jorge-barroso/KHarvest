#ifndef ADDEDTASKSLIST_H
#define ADDEDTASKSLIST_H

#include <QVector>
#include <QMap>
#include <QDate>
#include <QObject>

#include "task.h"

class AddedTasksList : public QObject {
Q_OBJECT
public:
    explicit AddedTasksList(QObject *parent = nullptr);

    [[nodiscard]] QVector<Task *> tasks() const;

    bool taskEdited(const int index, const Task *const task);

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
};

#endif // ADDEDTASKSLIST_H
