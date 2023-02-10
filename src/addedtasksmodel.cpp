#include "addedtasksmodel.h"
#include "addedtaskslist.h"
#include <QDebug>

AddedTasksModel::AddedTasksModel(QObject *parent)
        : QAbstractListModel(parent), mList(nullptr) {}

int AddedTasksModel::rowCount(const QModelIndex &parent) const {
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !mList)
        return 0;

    return mList->tasks().size();
}

QVariant AddedTasksModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || !mList)
        return {};

    switch (role) {
        case HeaderRole:
            return {mList->tasks().at(index.row())->get_project_label()};
        case SubtitleRole:
            return {mList->tasks().at(index.row())->taskName};
        case NoteRole:
            return {mList->tasks().at(index.row())->note};
        case TimeRole:
            return {mList->tasks().at(index.row())->timeTracked.toString("hh:mm")};
        case EntryIdRole:
            return {mList->tasks().at(index.row())->timeEntryId};
        case ProjectNameRole:
            return {mList->tasks().at(index.row())->projectName};
        default:
            return {};
    }
}

QHash<int, QByteArray> AddedTasksModel::roleNames() const {
    return {
            {HeaderRole,      "header"},
            {SubtitleRole,    "subtitle"},
            {NoteRole,        "note"},
            {TimeRole,        "time"},
            {EntryIdRole,     "entryId"},
            {ProjectNameRole, "project"},
    };
}

AddedTasksList *AddedTasksModel::list() const {
    return mList;
}

void AddedTasksModel::setList(AddedTasksList *list) {
    beginResetModel();

    if (mList)
        mList->disconnect(this);

    mList = list;

    if (mList != nullptr) {
        connect(mList, &AddedTasksList::preTaskAdded, this, [this]() {
            const int index = mList->tasks().size();
            beginInsertRows(QModelIndex(), index, index);
        });
        connect(mList, &AddedTasksList::postTaskAdded, this, [this]() {
            endInsertRows();
        });
        connect(mList, &AddedTasksList::preTaskRemoved, this, [this](const int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });
        connect(mList, &AddedTasksList::postTaskRemoved, this, [this]() {
            endRemoveRows();
        });
    }

    endResetModel();
}
