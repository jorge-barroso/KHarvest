#include "projectslist.h"

ProjectsList::ProjectsList(QObject *parent)
    : QObject{parent}
    , current_index{0}
{
    for (int i{1}; i<=3; ++i) {
        QVector<HarvestTask> tasks;
        for(int j{1}; j<=3; ++j) {
            const QString jString{QString::number(i*j)};
            tasks.append(HarvestTask{
                    .task_id=rand(),
                    .task_name="Test Task" + jString
            });
        }
        const QString iString{QString::number(i)};
        m_projects.append(HarvestProject{
                .company_name="Test Company " + iString,
                .project_name="Test Project " + iString,
                .project_id=rand(),
                .task={tasks}
        });
    }

    if(!m_projects.empty()) {
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

    if(index < 0 || index >= m_projects.size())
        return;

    if(index == current_index)
        return;

    current_index = index;

    const HarvestProject project{m_projects.at(index)};
    m_tasks = project.task;

    emit postTasksChanged();
}
