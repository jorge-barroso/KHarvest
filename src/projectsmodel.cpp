#include "projectsmodel.h"
#include "projectslist.h"
#include <QDebug>

ProjectsModel::ProjectsModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_list(nullptr)
{}

int ProjectsModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !m_list)
        return 0;

    return m_list->projects().size();
}

QVariant ProjectsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_list)
        return {};

    return (role == ProjectRole) ? m_list->projects().at(index.row()).get_project_label() : QVariant();
}

QHash<int, QByteArray> ProjectsModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[ProjectRole] = "projectName";
    return names;
}

ProjectsList *ProjectsModel::list() const {
    return m_list;
}

void ProjectsModel::setList(ProjectsList *list) {
    beginResetModel();

    if (m_list)
        m_list->disconnect(this);

    m_list = list;

    if (m_list != nullptr) {
        connect(m_list, &ProjectsList::projectsUpdated, this, &ProjectsModel::endResetModel);
    }

    endResetModel();
}
