#include "addedtasksmodel.h"
#include "addedtaskslist.h"
#include <QDebug>

AddedTasksModel::AddedTasksModel(QObject *parent)
        : QAbstractListModel(parent), m_list(nullptr) {}

int AddedTasksModel::rowCount(const QModelIndex &parent) const {
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !m_list)
        return 0;

    return m_list->tasks().size();
}

QVariant AddedTasksModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || !m_list)
        return {};

    switch (role) {
        case HeaderRole:
            return {m_list->tasks().at(index.row())->get_project_label()};
        case SubtitleRole:
            return {m_list->tasks().at(index.row())->task_name};
        case TimeRole:
            return {m_list->tasks().at(index.row())->time_tracked.toString("hh:mm")};
        default:
            return {};
    }
}

QHash<int, QByteArray> AddedTasksModel::roleNames() const {
    return {
            {HeaderRole, "header"},
            {SubtitleRole, "subtitle"},
            {NoteRole, "note"},
            {TimeRole, "time"}
    };
}

AddedTasksList *AddedTasksModel::list() const {
    return m_list;
}

void AddedTasksModel::setList(AddedTasksList *list) {
    beginResetModel();

    if (m_list)
        m_list->disconnect(this);

    m_list = list;

    if (m_list != nullptr) {
//        connect(m_list, &AddedTasksList::postTaskAdded, this, [this]() {
//            endInsertRows();
//        });
    }

    endResetModel();
}
