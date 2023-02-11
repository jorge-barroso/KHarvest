//
// Created by jorge on 11/02/23.
//

#include "favouriteslist.h"
#include "kharvestconfig.h"

FavouritesList::FavouritesList(QObject *parent)
        : QObject(parent), mFavourites() {
    mFavourites.reserve(KHarvestConfig::favourites_list().size());
    const QStringList favourites{KHarvestConfig::favourites_list()};
    std::for_each(favourites.begin(), favourites.end(), [this](const QString &favouriteString) {
        Task *task = new Task{};
        favouriteString >> *task;
        this->favouriteAdded(task);
    });
}

FavouritesList::~FavouritesList() {
    QStringList favouritesList;
    for (const Task *task: mFavourites) {
        favouritesList << task;
        delete task;
    }
    KHarvestConfig::setFavourites_list(favouritesList);
    KHarvestConfig::self()->save();
}

QVector<Task *> FavouritesList::favourites() const {
    return mFavourites;
}

void FavouritesList::favouriteAdded(Task *task) {
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
