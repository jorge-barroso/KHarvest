#ifndef ADDEDTASKSMODEL_H
#define ADDEDTASKSMODEL_H

#include <QAbstractListModel>

class AddedTasksList;

class AddedTasksModel : public QAbstractListModel {
Q_OBJECT
    Q_PROPERTY(AddedTasksList *list READ list WRITE setList)

public:
    explicit AddedTasksModel(QObject *parent = nullptr);

    enum {
        HeaderRole = Qt::UserRole,
        TaskNameRole,
        TaskIdRole,
        NoteRole,
        TimeLabelRole,
        TimeRole,
        ProjectNameRole,
        ProjectIdRole,
        ShouldAutoStopRole,
        StartedRole,
        FavouritedRole,
    };

    // Basic functionality:
    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    [[nodiscard]] bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const override;

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    [[nodiscard]] AddedTasksList *list() const;

    void setList(AddedTasksList *list);

private:
    AddedTasksList *mList;
};

#endif // ADDEDTASKSMODEL_H
