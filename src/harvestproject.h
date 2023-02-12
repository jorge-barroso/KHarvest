//
// Created by jorge on 17/02/2022.
//

#ifndef HARVESTTIMER_QT_CMAKE_HARVESTPROJECT_H
#define HARVESTTIMER_QT_CMAKE_HARVESTPROJECT_H


#include <QString>
#include <QVector>
#include "harvesttask.h"

struct HarvestProject {
    QString projectName;
    QString clientName;
    qlonglong projectId;
    QVector<HarvestTask> task;

    [[nodiscard]] QString get_project_label() const {
        return QString("%1 (%2)").arg(projectName, clientName);
    }
};


#endif //HARVESTTIMER_QT_CMAKE_HARVESTPROJECT_H
