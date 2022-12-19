// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: %{CURRENT_YEAR} %{AUTHOR} <%{EMAIL}>

import QtQuick 2.6
import QtQuick.Controls 2.3 as Controls
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kharvest 1.0

ListModel {
    id: favouritesModel

    ListElement {
        projectName: "Apple"
        taskName: "Apple"
        taskNote: "testNote"
        timeTracked: "00:00"
    }
    ListElement {
        projectName: "Pineapple"
        taskName: "Pineapple"
        taskNote: "testNote"
        timeTracked: "00:00"
    }
    ListElement {
        projectName: "Orange"
        taskName: "Orange"
        taskNote: "testNote"
        timeTracked: "00:00"
    }
}
