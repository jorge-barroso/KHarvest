//
// Created by jorge on 11/02/23.
//

#include "favouriteslist.h"
#include "kharvestconfig.h"

FavouritesList::FavouritesList(QObject *parent)
        : QObject(parent)
        , mFavourites() {
    mFavourites.reserve(KHarvestConfig::favourites_list().size());
    const QStringList favourites{KHarvestConfig::favourites_list()};
    std::for_each(favourites.begin(), favourites.end(), [this](const QString &favouriteString) {
        TaskPointer task = std::make_shared<Task>();
        favouriteString >> task;
        this->favouriteAdded(task);
    });
}

FavouritesList::~FavouritesList() {
    QStringList favouritesList;
    for (const TaskPointer &task: mFavourites) {
        favouritesList << task;
    }
    KHarvestConfig::setFavourites_list(favouritesList);
    KHarvestConfig::self()->save();
}

QVector<FavouritesList::TaskPointer> FavouritesList::favourites() const {
    return mFavourites;
}

void FavouritesList::favouriteAdded(const TaskPointer &task) {
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

void FavouritesList::favouriteRemoved(const QVector<TaskPointer>::const_iterator &taskReference) {
    const int index{static_cast<int>(std::distance(mFavourites.constBegin(), taskReference))};
    favouriteRemoved(index);
}

bool FavouritesList::isFavourited(const qlonglong projectId, const qlonglong taskId) const {
    const QVector<TaskPointer> &tasksVector = favourites();
    QVector<TaskPointer>::const_iterator matchingTask{
            std::find_if(tasksVector.constBegin(),
                         tasksVector.constEnd(),
                         [projectId, taskId](const TaskPointer &task) {
                             return task->projectId == projectId && task->taskId == taskId;
                         })
    };

    return matchingTask != tasksVector.end();
}
