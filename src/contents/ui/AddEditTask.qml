// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kharvest 1.0 as KHarvest

Kirigami.OverlaySheet{
    id: addEditTaskSheet

    // Sheet mode
    property string mode: "add"

    property int index: -1
    property var model: {}

    property alias projectIndex: projectField.currentIndex
    property alias taskIndex: taskField.currentIndex
    property alias taskNote: noteField.text
    property alias timeTracked: timeField.text

    property int initialIndex: 0

    function resetAndClose() {
        close();
        projectField.currentIndex = initialIndex;
        projectsList.setTasksFromProject(initialIndex)
        taskField.currentIndex = initialIndex;
        noteField.text = '';
        timeField.text = '';
    }

    header: Kirigami.Heading {
        text: mode === "add" ? i18nc("@title:window", "Add Task")
                             : i18nc("@title:window", "Edit Task")
    }

    Kirigami.FormLayout {
        Controls.ComboBox {
            id: projectField
            anchors {
                left: parent.left
                right: parent.right
            }
            model: KHarvest.ProjectsModel {
                list: projectsList
                onModelReset: {
                    projectField.currentIndex = initialIndex;
                }
            }
             onActivated: {
                model.list.setTasksFromProject(currentIndex);
                taskField.currentIndex = initialIndex;
             }
        }
        Controls.ComboBox {
            id: taskField
            anchors {
                left: parent.left
                right: parent.right
            }
            model: KHarvest.TasksModel{
                list: projectsList
                onModelReset: {
                    taskField.currentIndex = initialIndex;
                }
            }
        }

        Controls.TextArea {
            id: noteField
            anchors {
                left: parent.left
                right: parent.right
                top: taskField.bottom
                topMargin: 10
                bottom: timeField.top
                bottomMargin: 10
            }
            Layout.minimumHeight: Kirigami.Units.gridUnit * 10
            text: mode === "add" ? "" : taskNote
            placeholderText: i18nc("@label:textbox", "Add A Custom Note...")
        }

        Controls.TextField {
            id: timeField
            anchors.right: parent.right
            Kirigami.FormData.label: i18nc("@label:textbox", "Time:")
            inputMask: "00:00"
            inputMethodHints: Qt.ImhDigitsOnly
            placeholderText: i18n("HH:MM")
            text: mode === "edit" ? "00:00" : timeTracked
        }
    }

    footer: RowLayout {
            anchors {
                left: parent.left
                right: parent.right
            }
            Controls.Button {
                id: doneButton
                Layout.fillWidth: true
                flat: true
                text: i18nc("@action:button", "Done")
                enabled: projectField.text.length > 0 && taskField.text.length > 0
                onClicked: {
                    if(mode === "add") {
                        KHarvest.TasksManager.newTaskAdded(projectIndex,
                                                taskIndex,
                                                taskNote,
                                                timeTracked);
                    }
                    else {
                        KHarvest.TasksManager.taskUpdated(index,
                                                        projectIndex,
                                                        taskIndex,
                                                        taskNote,
                                                        timeTracked);
                        model.project = projectField.currentText
                        model.subtitle = taskField.currentText
                        model.note = noteField.text
                        model.time = timeField.text
                    }
                    resetAndClose();
                }
            }
            Controls.Button {
                id: removeButton
                Layout.fillWidth: true
                flat: true
                text: i18nc("@action:button", "Remove")
                visible: mode === "edit"
                onClicked: {
                    addedTasksList.taskRemoved(index);
                    resetAndClose();
                }
            }
            Controls.Button {
                id: toFavouritesButton
                Layout.fillWidth: true
                flat: true
                text: i18nc("@action:button", "Favourites")
                visible: mode === "add"
                onClicked: {
                    addEditTaskSheet.favourited(index,
                                                projectField.text,
                                                taskField.text,
                                                noteField.text,
                                                timeField.text)
                    resetAndClose();
                }
            }
            Controls.Button {
                id: cancelButton
                Layout.fillWidth: true
                flat: true
                text: i18nc("@action:button", "Cancel")
                onClicked: {
                    resetAndClose();
                }
            }
        }
}
