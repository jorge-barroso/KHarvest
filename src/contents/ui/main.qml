// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.20 as Kirigami
import org.kde.kharvest 1.0 as KHarvest

Kirigami.ApplicationWindow {
    id: root

    title: i18n("Harvest Timer")

    minimumWidth: Kirigami.Units.gridUnit * 25
    minimumHeight: Kirigami.Units.gridUnit * 40

    onClosing: KHarvest.WindowController.saveWindowGeometry(root)

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
        onTriggered: KHarvest.WindowController.saveWindowGeometry(root)
    }

    property var runningTask: undefined

    Connections {
        target: KHarvest.HarvestHandler

        function onReady() {
            applicationWindow().pageStack.replace(page);
        }

        function onHarvestError(errorText = "") {
            errorMessage.text = errorText
            errorMessage.visible = true
        }

        function onHarvestWarning(warningText = "") {
            console.log("warning received");
            warningMessage.text = warningText
            warningMessage.visible = true
        }
    }

    function openEditTaskSheet(index = -1, model = {}) {
        addEditTaskSheet.mode = "edit"
        addEditTaskSheet.index = index;
        addEditTaskSheet.projectIndex = projectsList.projectIndexByLabel(model.header);
        projectsList.setTasksFromProject(addEditTaskSheet.projectIndex);
        addEditTaskSheet.taskIndex = projectsList.taskIndexByName(model.subtitle);
        addEditTaskSheet.taskNote = model.note;
        addEditTaskSheet.timeTracked = model.timeLabel;
        addEditTaskSheet.model = model;
        addEditTaskSheet.open()
    }

    function openAddTaskSheet(favouritesModel = undefined) {
        addEditTaskSheet.mode = "add"
        if(favouritesModel !== undefined) {
            console.log("adding a favourite");
            addEditTaskSheet.projectIndex = projectsList.projectIndexById(favouritesModel.projectId);
            projectsList.setTasksFromProject(addEditTaskSheet.projectIndex);
            addEditTaskSheet.taskIndex = projectsList.taskIndexById(favouritesModel.taskId);
        } else {
            console.log("adding a new task");
            addEditTaskSheet.projectIndex = 0;
            addEditTaskSheet.taskIndex = 0;
        }

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
               id: reloadUserDataAction
               text: i18nc("@action:inmenu", "Reload User Data")
               icon.name: "view-refresh"
               onTriggered: {

               }
           },
            Kirigami.Action {
               id: reloadTasksAction
               text: i18nc("@action:inmenu", "Reload Tasks")
               icon.name: "view-refresh"
               onTriggered: {
                addedTasksList.reload();
               }
           },
            Kirigami.Action {
                id: aboutAction
                text: i18nc("@action:inmenu", "About KHarvest")
                icon.name: "help-about"
                onTriggered: pageStack.layers.push(root.isMobile ? 'qrc:AboutMobile.qml' : 'qrc:About.qml')
            },
            Kirigami.Action {
               id: logoutAction
               text: i18nc("@action:inmenu", "Logout")
               icon.name: "list-remove-user"
               onTriggered: {
                   KHarvest.App.logout();
                   pageStack.replace('qrc:LoginPage.qml');
               }
           },
           Kirigami.Action {
                id: quitAction
                text: i18nc("@action:inmenu", "Quit")
				shortcut: StandardKey.Quit
                icon.name: "application-exit"
                onTriggered: Qt.quit()
            }
        ]
    }

    footer: ColumnLayout {
        Kirigami.InlineMessage {
            id: errorMessage
            Layout.fillWidth: true
            showCloseButton: true
            visible: false
            type: Kirigami.MessageType.Error
        }
        Kirigami.InlineMessage {
            id: warningMessage
            Layout.fillWidth: true
            showCloseButton: true
            visible: false
            type: Kirigami.MessageType.Warning
        }
        Kirigami.InlineMessage {
            Layout.fillWidth: true
            text: i18nc("InlineMessage when the system is offline",
                        "KHarvest is offline. Please check your network connection.")
            visible: !KHarvest.HarvestHandler.isOnline
        }
    }

    contextDrawer: Kirigami.ContextDrawer {
        id: contextDrawer
    }

    AddEditTask {
        id: addEditTaskSheet
    }

    MainPage {
        id: page
    }

    Component.onCompleted: {
        KHarvest.WindowController.restoreWindowGeometry(root)
        if (KHarvest.HarvestHandler.isReady) {
            logoutAction.visible = true
            pageStack.push(page);
        } else {
            logoutAction.visible = false
            pageStack.push('qrc:LoginPage.qml');
        }
    }
}
