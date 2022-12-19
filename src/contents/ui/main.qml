// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: %{CURRENT_YEAR} %{AUTHOR} <%{EMAIL}>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kharvest 1.0

Kirigami.ApplicationWindow {
    id: root

    title: i18n("Harvest Timer")

    minimumWidth: Kirigami.Units.gridUnit * 20
    minimumHeight: Kirigami.Units.gridUnit * 40

    onClosing: App.saveWindowGeometry(root)

    onWidthChanged: saveWindowGeometryTimer.restart()
    onHeightChanged: saveWindowGeometryTimer.restart()
    onXChanged: saveWindowGeometryTimer.restart()
    onYChanged: saveWindowGeometryTimer.restart()

    Component.onCompleted: App.restoreWindowGeometry(root)

    // This timer allows to batch update the window size change to reduce
    // the io load and also work around the fact that x/y/width/height are
    // changed when loading the page and overwrite the saved geometry from
    // the previous session.
    Timer {
        id: saveWindowGeometryTimer
        interval: 1000
        onTriggered: App.saveWindowGeometry(root)
    }

    function openPopulateSheet(mode, index = -1, projectName = "", taskName = "", taskNote = "", timeTracked = "") {
        addEditTaskSheet.mode = mode
        if(mode === "edit") {
            addEditTaskSheet.index = index;
            addEditTaskSheet.projectName = projectName;
            addEditTaskSheet.taskName = taskName;
            addEditTaskSheet.taskNote = taskNote;
            addEditTaskSheet.timeTracked = timeTracked;
        }
        addEditTaskSheet.open()
    }

    function deleteTask(index) {
        taskModel.remove(index)
    }

    function unfavTask(index) {
        favouritesModel.remove(index)
    }

    globalDrawer: Kirigami.GlobalDrawer {
        title: i18n("KHarvest")
        titleIcon: "applications-graphics"
        isMenu: !root.isMobile
        actions: [
            Kirigami.Action {
                text: i18n("Add Task")
                icon.name: "list-add"
                onTriggered: openPopulateSheet("add")
            },
            Kirigami.Action {
                text: i18n("About KHarvest")
                icon.name: "help-about"
                onTriggered: pageStack.layers.push('qrc:About.qml')
            },
            Kirigami.Action {
                text: i18n("Quit")
                icon.name: "application-exit"
                onTriggered: Qt.quit()
            }
        ]
    }

    contextDrawer: Kirigami.ContextDrawer {
        id: contextDrawer
    }

    TaskModel {
        id: taskModel
    }

    FavouritesModel {
        id: favouritesModel
    }

    MainPage {
        id: page
    }

    AddEditTask{
        id: addEditTaskSheet
        onAdded: taskModel.append({
            "projectName": projectName,
            "taskName": taskName,
            "taskNote": taskNote
        })
        onEdited: taskModel.set(index, {
            "projectName": projectName,
            "taskName": taskName,
            "taskNote": taskNote
        })
        onRemoved: taskModel.remove(index, 1)
    }

    pageStack.initialPage: page
}
