//
// Created by jorge on 11/02/23.
//

#include "favouriteslist.h"
#include "kharvestconfig.h"

FavouritesList::FavouritesList(QObject *parent)
        : QObject(parent), mFavourites(KHarvestConfig::favourites_list().size()) {
    mFavourites.append(new Task{
            .projectId=123432,
            .taskId=657621,
            .clientName="Sample Client",
            .projectName="Sample Project",
            .taskName = "Sample Task",
            .note="Note",
    });

    const QStringList favourites{KHarvestConfig::favourites_list()};
    std::for_each(favourites.begin(),
                  favourites.end(),
                  [this](const QString &taskLine) {
                      Task *task = new Task{};
                      taskLine >> *task;
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
    emit postFavouriteAdded();
}
