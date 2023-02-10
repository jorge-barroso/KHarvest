#include "tasksmodel.h"
#include "projectslist.h"

TasksModel::TasksModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_list(nullptr)
{}

int TasksModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !m_list)
        return 0;

    return m_list->tasks().size();
}

QVariant TasksModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_list)
        return {};

    return role == TaskRole ? m_list->tasks().at(index.row()).task_name : QVariant();
}

QHash<int, QByteArray> TasksModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[TaskRole] = "taskName";
    return names;
}

ProjectsList *TasksModel::list() const {
    return m_list;
}

void TasksModel::setList(ProjectsList *list) {
    beginResetModel();

    if (m_list)
        m_list->disconnect(this);

    m_list = list;

    if (m_list)
        connect(m_list, &ProjectsList::postTasksChanged, this, &TasksModel::endResetModel);

    endResetModel();
}

