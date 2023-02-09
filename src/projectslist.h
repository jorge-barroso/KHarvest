#ifndef PROJECTSLIST_H
#define PROJECTSLIST_H

#include <QObject>
#include <vector>
#include "harvestproject.h"

class ProjectsList : public QObject
{
    Q_OBJECT
public:
    explicit ProjectsList(QObject *parent = nullptr);

    [[nodiscard]] QVector<HarvestProject> projects() const;
    [[nodiscard]] QVector<HarvestTask> tasks() const;

    void setTasksFromProject(int index);
signals:
    void preTaskChanged();
    void postTasksChanged();
//
//public slots:
//    void projectChanged();
private:
    QVector<HarvestProject> m_projects;
    QVector<HarvestTask> m_tasks;
    int current_index;
};

#endif // PROJECTSLIST_H
