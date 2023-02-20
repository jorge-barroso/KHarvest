// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

#include <QtConcurrent>
#include "projectslist.h"
#include "harvesthandler.h"
#include "maputils.h"

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
        cachedTasksByName.reserve(m_projects.size());
        for(int i = 0; i < m_projects.size(); ++i) {
            const HarvestProject &currentProject{m_projects.at(i)};
            cachedProjectsById.insert(currentProject.projectId, static_cast<long>(i));
            cachedProjectsByLabel.insert(currentProject.get_project_label(), static_cast<long>(i));
            QMap<QString, long> tasksByNameMap;
            for(int j = 0; j < currentProject.task.size(); ++j) {
                HarvestTask task{currentProject.task.at(j)};
                cachedTasksById.insert(task.task_id, j);
                tasksByNameMap.insert(task.task_name, j);
            }
            cachedTasksByName.append(tasksByNameMap);
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
    const auto projectIt{cachedProjectsById.constFind(projectId)};
    if(projectIt != cachedProjectsById.constEnd()) {
        return projectIt.value();
    }

    // It's not in the cache (too quick after startup?) so we'll do a linear search
    const HarvestProject *project = std::find_if(m_projects.constBegin(), m_projects.constBegin(),
                                                 [projectId](const HarvestProject &project) {
                                                     return project.projectId == projectId;
                                                 });
    const long index{std::distance(m_projects.constBegin(), project)};

    return index;
}

long ProjectsList::taskIndexById(qlonglong taskId) const {
    const auto taskIt{cachedTasksById.constFind(taskId)};
    if(taskIt != cachedTasksById.constEnd()) {
        return taskIt.value();
    }

    // It's not in the cache (too quick after startup?) so we'll do a linear search
    const HarvestTask *task = std::find_if(m_tasks.constBegin(), m_tasks.constEnd(),
                                           [taskId](const HarvestTask &task) {
                                               return task.task_id == taskId;
                                           });
    const long index{std::distance(m_tasks.constBegin(), task)};

    return index;
}

long ProjectsList::projectIndexByLabel(const QString &projectLabel) {
    const auto projectIt{cachedProjectsByLabel.constFind(projectLabel)};
    if(projectIt != cachedProjectsByLabel.constEnd()) {
        return projectIt.value();
    }

    // It's not in the cache (too quick after startup?) so we'll do a linear search
    const HarvestProject *project = std::find_if(m_projects.constBegin(), m_projects.constBegin(),
                                                 [projectLabel](const HarvestProject &project) {
                                                     return project.get_project_label() == projectLabel;
                                                 });
    const long index{std::distance(m_projects.constBegin(), project)};

    return index;
}



long ProjectsList::taskIndexByName(const QString& taskName) const {
    const IndexesByNameMap &tasksMap = cachedTasksByName.at(current_index);
    const auto taskIt{tasksMap.constFind(taskName)};
    if(taskIt != tasksMap.constEnd()) {
        return taskIt.value();
    }

    // It's not in the cache (too quick after startup?) so we'll do a linear search
    const HarvestTask *task = std::find_if(m_tasks.constBegin(), m_tasks.constEnd(),
                                           [taskName](const HarvestTask &task) {
                                               return task.task_name == taskName;
                                           });
    const long index{std::distance(m_tasks.constBegin(), task)};

    return index;
}