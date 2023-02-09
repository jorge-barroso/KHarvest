#ifndef PROJECTSLIST_H
#define PROJECTSLIST_H

#include <QObject>
#include <vector>
#include "harvestproject.h"

static const int INITIAL_INDEX = 0;

class ProjectsList : public QObject {
Q_OBJECT
public:
    explicit ProjectsList(QObject *parent = nullptr);

    [[nodiscard]] QVector<HarvestProject> projects() const;

    [[nodiscard]] QVector<HarvestTask> tasks() const;

signals:

    void projectsUpdated();

    void preTaskChanged();

    void postTasksChanged();
//
public slots:

    void setTasksFromProject(int index);
//    void projectChanged();
private:
    QVector<HarvestProject> m_projects;
    QVector<HarvestTask> m_tasks;
    int current_index;
};

#endif // PROJECTSLIST_H
