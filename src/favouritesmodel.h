#ifndef KHARVEST_FAVOURITESMODEL_H
#define KHARVEST_FAVOURITESMODEL_H

#include <QAbstractListModel>

class FavouritesList;

class FavouritesModel : public QAbstractListModel {
Q_OBJECT
    Q_PROPERTY(FavouritesList *list READ list WRITE setList)

public:
    explicit FavouritesModel(QObject *parent = nullptr);

    enum {
        HeaderRole = Qt::UserRole,
        ProjectNameRole,
        ProjectIdRole,
        TaskNameRole,
        TaskIdRole,
    };

    // Basic functionality:
    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    [[nodiscard]] FavouritesList *list() const;

    void setList(FavouritesList *list);

private:
    FavouritesList *mList;
};


#endif //KHARVEST_FAVOURITESMODEL_H
