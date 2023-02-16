// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

#ifndef HARVESTTIMER_QT_CMAKE_HARVESTTASK_H
#define HARVESTTIMER_QT_CMAKE_HARVESTTASK_H


#include <QString>

struct HarvestTask
{
    qlonglong task_id;
    QString task_name;
    QString client_name;
};


#endif //HARVESTTIMER_QT_CMAKE_HARVESTTASK_H
