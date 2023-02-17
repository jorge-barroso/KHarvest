// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

#include "favouriteslist.h"
#include "kharvestconfig.h"

FavouritesList::FavouritesList(QObject *parent)
        : QObject(parent)
        , mFavourites() {
    const QJsonArray& favouritesArray{QJsonDocument::fromJson(KHarvestConfig::favourites_list().toUtf8()).array()};
    mFavourites.reserve(favouritesArray.size());
    std::for_each(favouritesArray.begin(), favouritesArray.end(), [this](const QJsonValue &favouriteString) {
        TaskPtr task = std::make_shared<Task>();
        favouriteString.toObject() >> task;
        this->favouriteAdded(task);
    });
}

FavouritesList::~FavouritesList() {
    QJsonArray newFavouritesArray;
    for (const TaskPtr &task: mFavourites) {
        newFavouritesArray << task;
    }
    KHarvestConfig::setFavourites_list(QJsonDocument(newFavouritesArray).toJson(QJsonDocument::JsonFormat::Compact));
    KHarvestConfig::self()->save();
}

QVector<FavouritesList::TaskPtr> FavouritesList::favourites() const {
    return mFavourites;
}

void FavouritesList::favouriteAdded(const TaskPtr &task) {
    emit preFavouriteAdded();
    mFavourites.append(task);
    emit postFavouriteAdded();
}

void FavouritesList::favouriteRemoved(int index) {
    if (index < 0 || index >= mFavourites.size()) {
        return;
    }

    emit preFavouriteRemoved(index);
    mFavourites.remove(index);
    emit postFavouriteRemoved();
}

void FavouritesList::favouriteRemoved(const QVector<TaskPtr>::const_iterator &taskReference) {
    const int index{static_cast<int>(std::distance(mFavourites.constBegin(), taskReference))};
    favouriteRemoved(index);
}

bool FavouritesList::isFavourited(const qlonglong projectId, const qlonglong taskId) const {
    const QVector<TaskPtr> &tasksVector = favourites();
    QVector<TaskPtr>::const_iterator matchingTask{
            std::find_if(tasksVector.constBegin(),
                         tasksVector.constEnd(),
                         [projectId, taskId](const TaskPtr &task) {
                             return task->projectId == projectId && task->taskId == taskId;
                         })
    };

    return matchingTask != tasksVector.end();
}
