#ifndef PROJECTSMODEL_H
#define PROJECTSMODEL_H

#include <QAbstractListModel>
#include "harvestproject.h"

class ProjectsList;

class ProjectsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(ProjectsList *list READ list WRITE setList)

public:
    explicit ProjectsModel(QObject *parent = nullptr);

    enum {
        ProjectRole = Qt::UserRole,
        TaskRole
    };

    // Basic functionality:
    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    [[nodiscard]] ProjectsList *list() const;

    void setList(ProjectsList *list);

public slots:
    void newProjectChosen(int new_index);

private:
    ProjectsList *m_list;
};

#endif // PROJECTSMODEL_H
