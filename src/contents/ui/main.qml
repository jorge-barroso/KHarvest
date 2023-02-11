// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kharvest 1.0 as KHarvest

Kirigami.ApplicationWindow {
    id: root

    title: i18n("Harvest Timer")

    minimumWidth: Kirigami.Units.gridUnit * 25
    minimumHeight: Kirigami.Units.gridUnit * 40

    onClosing: KHarvest.App.saveWindowGeometry(root)

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
        onTriggered: KHarvest.App.saveWindowGeometry(root)
    }

    property var runningTask: undefined

    footer: ColumnLayout {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Controls.Label {
                    id: dateLabel
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    font.bold: true
                    text: "10 February 2023"
                }
                id: dateToolBar
                visible: KHarvest.HarvestHandler.isReady && page.isCurrentPage
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
        target: KHarvest.HarvestHandler

        function onReady() {
            applicationWindow().pageStack.replace(page);
            dateToolBar.visible = true
        }
    }

    function openEditTaskSheet(index = -1, model = {}) {
        addEditTaskSheet.mode = "edit"
        addEditTaskSheet.index = index;
        addEditTaskSheet.projectIndex = KHarvest.TasksManager.projectIndexByName(model.header);
        projectsList.setTasksFromProject(addEditTaskSheet.projectIndex);
        addEditTaskSheet.taskIndex = KHarvest.TasksManager.taskIndexByName(model.subtitle);
        addEditTaskSheet.taskNote = model.note;
        addEditTaskSheet.timeTracked = model.timeLabel;
        addEditTaskSheet.model = model;
        addEditTaskSheet.open()
    }

    function openAddTaskSheet() {
        addEditTaskSheet.mode = "add"
        addEditTaskSheet.projectIndex = 0;
        addEditTaskSheet.taskIndex = 0;
        addEditTaskSheet.taskNote = "";
        addEditTaskSheet.timeTracked = "00:00";
        addEditTaskSheet.open()
    }

    globalDrawer: Kirigami.GlobalDrawer {
        id: globalDrawer
        title: i18n("KHarvest")
        titleIcon: "applications-graphics"
        isMenu: !root.isMobile
        actions: [
            Kirigami.Action {
                id: aboutAction
                text: i18n("About KHarvest")
                icon.name: "help-about"
                onTriggered: pageStack.layers.push('qrc:About.qml')
            },
            Kirigami.Action {
               id: logoutAction
               text: i18n("Logout")
               icon.name: "list-remove-user"
               onTriggered: {
                   KHarvest.App.logout();
                   pageStack.replace('qrc:LoginPage.qml');
               }
           },
           Kirigami.Action {
                id: quitAction
                text: i18n("Quit")
                icon.name: "application-exit"
                onTriggered: Qt.quit()
            }
        ]
    }

    contextDrawer: Kirigami.ContextDrawer {
        id: contextDrawer
    }

    AddEditTask{
        id: addEditTaskSheet
    }

    MainPage {
        id: page
        Component.onCompleted: addEditTaskSheet.parent = page
    }

    Component.onCompleted: {
        KHarvest.App.restoreWindowGeometry(root)
        if (KHarvest.HarvestHandler.isReady) {
            logoutAction.visible = true
            pageStack.push(page);
        } else {
            logoutAction.visible = false
            pageStack.push('qrc:LoginPage.qml');
        }
    }
}
