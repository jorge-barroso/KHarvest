#ifndef TASKSMODEL_H
#define TASKSMODEL_H

#include <QAbstractListModel>
#include "harvestproject.h"

class ProjectsList;

class TasksModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(ProjectsList *list READ list WRITE setList)

public:
    explicit TasksModel(QObject *parent = nullptr);

    enum {
        TaskRole = Qt::UserRole,
    };

    // Basic functionality:
    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    [[nodiscard]] ProjectsList *list() const;
    void setList(ProjectsList *list);
private:
    ProjectsList *m_list;
};

#endif // TASKSMODEL_H
