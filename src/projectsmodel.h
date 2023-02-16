// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

#ifndef PROJECTSMODEL_H
#define PROJECTSMODEL_H

#include <QAbstractListModel>

class ProjectsList;

class ProjectsModel : public QAbstractListModel {
Q_OBJECT
    Q_PROPERTY(ProjectsList *list READ list WRITE setList)

public:
    explicit ProjectsModel(QObject *parent = nullptr);

    enum {
        ProjectNameRole = Qt::UserRole,
        ProjectLabelRole,
    };

    // Basic functionality:
    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    [[nodiscard]] ProjectsList *list() const;

    void setList(ProjectsList *list);

private:
    ProjectsList *mList;
};

#endif // PROJECTSMODEL_H
