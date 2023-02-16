// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

#include <QtConcurrent>
#include "projectslist.h"
#include "harvesthandler.h"

ProjectsList::ProjectsList(QObject *parent)
        : QObject{parent}
        , current_index{-1} {

    const std::shared_ptr<HarvestHandler>& harvest_handler{HarvestHandler::instance()};
    if (harvest_handler->is_ready()) {
        loadProjects();
    } else {
        connect(harvest_handler.get(), &HarvestHandler::ready, this, &ProjectsList::loadProjects);
    }
}

void ProjectsList::loadProjects() {
    m_projects = HarvestHandler::instance()->update_user_data();
    QtConcurrent::run([this]{
        for(int i = 0; i < m_projects.size(); ++i) {
            cachedProjects.insert(m_projects.at(i).projectId, static_cast<long>(i));
        }
    });

    emit projectsUpdated();
    setTasksFromProject(INITIAL_INDEX);
}

QVector<HarvestProject> ProjectsList::projects() const {
    return m_projects;
}

QVector<HarvestTask> ProjectsList::tasks() const {
    return m_tasks;
}

void ProjectsList::setTasksFromProject(const int index) {
    emit preTaskChanged();

    if (index < 0 || index >= m_projects.size())
        return;

    if (index == current_index)
        return;

    current_index = index;

    const HarvestProject project{m_projects.at(index)};
    m_tasks = project.task;

    emit postTasksChanged();
}

long ProjectsList::projectIndexById(qlonglong projectId) const {
    const auto projectIt{cachedProjects.constFind(projectId)};
    if(projectIt == cachedProjects.constEnd()) {
        // It's not in the cache (too quick after startup?) so we'll do a linear search
        const HarvestProject *project = std::find_if(m_projects.begin(), m_projects.end(),
                                                     [projectId](const HarvestProject &project) {
                                                         return project.projectId == projectId;
                                                     });
        const long index{std::distance(m_projects.begin(), project)};

        return index;
    }
    return projectIt.value();
}
