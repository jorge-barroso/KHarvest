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

signals:

    void preTaskAdded();

    void postTaskAdded();

public slots:

    void task_added(Task *task);

private:
    QMap<QDate, QVector<Task *>> mTasks;
    QDate current_date;
};

#endif // ADDEDTASKSLIST_H
