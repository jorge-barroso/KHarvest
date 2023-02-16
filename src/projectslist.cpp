// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

#include "projectslist.h"
#include "harvesthandler.h"

ProjectsList::ProjectsList(QObject *parent)
        : QObject{parent}, current_index{-1} {

    const std::shared_ptr<HarvestHandler>& harvest_handler{HarvestHandler::instance()};
    if (harvest_handler->is_ready()) {
        current_index = INITIAL_INDEX;
        m_projects = harvest_handler->update_user_data();
    } else {
        connect(harvest_handler.get(), &HarvestHandler::ready, this, [this, harvest_handler] {
            m_projects = harvest_handler->update_user_data();
            emit projectsUpdated();
            setTasksFromProject(INITIAL_INDEX);
        });
    }

    if (!m_projects.empty()) {
        m_tasks = m_projects.at(current_index).task;
    }
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
