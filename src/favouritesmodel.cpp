// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

#include "favouritesmodel.h"
#include "favouriteslist.h"

FavouritesModel::FavouritesModel(QObject *parent)
        : QAbstractListModel(parent), mList(nullptr) {}

int FavouritesModel::rowCount(const QModelIndex &parent) const {
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !mList)
        return 0;

    return mList->favourites().size();
}

QVariant FavouritesModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || !mList)
        return {};

    const std::shared_ptr<Task>& task{mList->favourites().at(index.row())};
    switch (role) {
        case HeaderRole:
            return {task->get_project_label()};
        case ProjectNameRole:
            return {task->projectName};
        case ProjectIdRole:
            return {task->projectId};
        case TaskNameRole:
            return {task->taskName};
        case TaskIdRole:
            return {task->taskId};
        default:
            return {};
    }
}

QHash<int, QByteArray> FavouritesModel::roleNames() const {
    return {
            {HeaderRole,      "header"},
            {ProjectNameRole, "projectName"},
            {ProjectIdRole,   "projectId"},
            {TaskNameRole,    "taskName"},
            {TaskIdRole,      "taskId"},
    };
}

FavouritesList *FavouritesModel::list() const {
    return mList;
}

void FavouritesModel::setList(FavouritesList *list) {
    beginResetModel();

    if (mList)
        mList->disconnect(this);

    mList = list;

    if (mList != nullptr) {
        connect(mList, &FavouritesList::preFavouriteAdded, this, [this]() {
            const int index{mList->favourites().size()};
            beginInsertRows(QModelIndex(), index, index);
        });
        connect(mList, &FavouritesList::postFavouriteAdded, this, [this]() {
            endInsertRows();
        });
        connect(mList, &FavouritesList::preFavouriteRemoved, this, [this](const int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });
        connect(mList, &FavouritesList::postFavouriteRemoved, this, [this]() {
            endRemoveRows();
        });
    }

    endResetModel();
}
