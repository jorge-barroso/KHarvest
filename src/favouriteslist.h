// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

#ifndef KHARVEST_FAVOURITESLIST_H
#define KHARVEST_FAVOURITESLIST_H


#include <QObject>
#include <memory>
#include "task.h"

class FavouritesList : public QObject {
    typedef std::shared_ptr<Task> TaskPtr;
Q_OBJECT

public:
    explicit FavouritesList(QObject *parent = nullptr);

    ~FavouritesList() override;

    [[nodiscard]] QVector<TaskPtr> favourites() const;

    [[nodiscard]] bool isFavourited(qlonglong projectId, qlonglong taskId) const;

    void favouriteRemoved(const QVector<TaskPtr>::const_iterator &taskReference);

signals:

    void preFavouriteAdded();

    void postFavouriteAdded();

    void preFavouriteRemoved(int index);

    void postFavouriteRemoved();

public slots:

    void favouriteAdded(const TaskPtr& task);

    void favouriteRemoved(int index);

private:
    QVector<TaskPtr> mFavourites;
};


#endif //KHARVEST_FAVOURITESLIST_H
