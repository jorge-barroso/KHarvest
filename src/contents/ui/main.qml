// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

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

    // This timer allows to batch update the window size change to reduce
    // the io load and also work around the fact that x/y/width/height are
    // changed when loading the page and overwrite the saved geometry from
    // the previous session.
    Timer {
        id: saveWindowGeometryTimer
        interval: 1000
        onTriggered: App.saveWindowGeometry(root)
    }

    footer: ColumnLayout {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Controls.Label {
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    font.bold: true
                    text: "09 February 2023"
                }
                id: dateToolBar
                visible: HarvestHandler.isReady
                Kirigami.ActionToolBar {
                    alignment: Qt.AlignCenter
                    flat: true
                    actions: [
                       Kirigami.Action {
                           icon.name: "go-previous"
                           onTriggered: {}
                       },
                       Kirigami.Action {
                           icon.name: "view-calendar"
                           onTriggered: {}
                       },
                       Kirigami.Action {
                           icon.name: "go-next"
                           onTriggered: {}
                       }
                   ]
                }
            }

    Connections {
        target: HarvestHandler

        function onReady() {
            applicationWindow().pageStack.replace(page)
            dateToolBar.visible = true
        }
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

    MainPage {
        id: page
        Component.onCompleted: addEditTaskSheet.parent = page
    }

    Component.onCompleted: {
        App.restoreWindowGeometry(root)
        if (HarvestHandler.isReady) {
            pageStack.push(page);
        } else {
            pageStack.push('qrc:/content/ui/LoginPage.qml');
        }
    }
}
