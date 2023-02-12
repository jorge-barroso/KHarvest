#include "projectsmodel.h"
#include "projectslist.h"

ProjectsModel::ProjectsModel(QObject *parent)
    : QAbstractListModel(parent), mList(nullptr)
{}

int ProjectsModel::rowCount(const QModelIndex &parent) const {
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !mList)
        return 0;

    return mList->projects().size();
}

QVariant ProjectsModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || !mList)
        return {};

    const HarvestProject &project = mList->projects().at(index.row());
    switch (role) {
        case ProjectNameRole:
            return project.projectName;
        case ProjectLabelRole:
            return project.get_project_label();
        default:
            return {};
    }
}

QHash<int, QByteArray> ProjectsModel::roleNames() const {
    QHash<int, QByteArray> names;
    names[ProjectNameRole] = "projectName";
    names[ProjectLabelRole] = "projectLabel";
    return names;
}

ProjectsList *ProjectsModel::list() const {
    return mList;
}

void ProjectsModel::setList(ProjectsList *list) {
    beginResetModel();

    if (mList)
        mList->disconnect(this);

    mList = list;

    if (mList != nullptr) {
        connect(mList, &ProjectsList::projectsUpdated, this, &ProjectsModel::endResetModel);
    }

    endResetModel();
}
