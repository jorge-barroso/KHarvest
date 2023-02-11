//
// Created by jorge on 11/02/23.
//

#ifndef KHARVEST_FAVOURITESLIST_H
#define KHARVEST_FAVOURITESLIST_H


#include <QObject>
#include "task.h"

class FavouritesList : public QObject {
Q_OBJECT

public:
    explicit FavouritesList(QObject *parent = nullptr);

    ~FavouritesList() override;

    [[nodiscard]] QVector<Task *> favourites() const;

signals:

    void preFavouriteAdded();

    void postFavouriteAdded();

    void preFavouriteRemoved(int index);

    void postFavouriteRemoved();

public slots:

    void favouriteAdded(Task *task);

    void favouriteRemoved(int index);

private:
    QVector<Task *> mFavourites;
};


#endif //KHARVEST_FAVOURITESLIST_H
