//
// Created by jorge on 17/02/2022.
//

#ifndef HARVESTTIMER_QT_CMAKE_HARVESTPROJECT_H
#define HARVESTTIMER_QT_CMAKE_HARVESTPROJECT_H


#include <QString>
#include <QVector>
#include "harvesttask.h"

struct HarvestProject {
    QString project_name;
    QString client_name;
    qint64 project_id;
    QVector<HarvestTask> task;

    [[nodiscard]] QString get_project_label() const {
        return project_name + " (" + client_name + ")";
    }
};


#endif //HARVESTTIMER_QT_CMAKE_HARVESTPROJECT_H
