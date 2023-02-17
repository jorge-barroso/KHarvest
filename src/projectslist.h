// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

#ifndef PROJECTSLIST_H
#define PROJECTSLIST_H

#include <QObject>
#include <vector>
#include <QMap>
#include "harvestproject.h"

static const int INITIAL_INDEX = 0;

class ProjectsList : public QObject {
Q_OBJECT

    typedef QMap<qlonglong, long> IndexesByIdMap;
    typedef QMap<QString, long> IndexesByNameMap;
public:
    explicit ProjectsList(QObject *parent = nullptr);

    [[nodiscard]] QVector<HarvestProject> projects() const;

    [[nodiscard]] long projectIndexById(qlonglong projectId) const;

    [[nodiscard]] long projectIndexByLabel(const QString &projectLabel);

    [[nodiscard]] long taskIndexById(qlonglong taskId) const;

    [[nodiscard]] long taskIndexByName(const QString &taskName) const;

    [[nodiscard]] QVector<HarvestTask> tasks() const;

signals:

    void projectsUpdated();

    void preTaskChanged();

    void postTasksChanged();

public slots:

    void setTasksFromProject(int index);

private:
    QVector<HarvestProject> m_projects;
    QVector<HarvestTask> m_tasks;
    IndexesByIdMap cachedProjectsById;
    IndexesByIdMap cachedTasksById;
    IndexesByNameMap cachedProjectsByLabel;
    QVector<IndexesByNameMap> cachedTasksByName;
    int current_index;

    void loadProjects();
};

#endif // PROJECTSLIST_H
