// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: %{CURRENT_YEAR} %{AUTHOR} <%{EMAIL}>

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
    property string projectName: ""
    property string taskName: ""
    property string taskNote: ""
    property string timeTracked: ""

    signal added (string projectName, string taskName, string taskNote)
    signal edited (int index, string projectName, string taskName, string taskNote)
    signal removed(int index)

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
            }
             onCurrentIndexChanged: {
                model.newProjectChosen(currentIndex)
                taskField.currentIndex = 0
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
            text: mode === "add" ? "00:00" : timeTracked
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
                        addEditTaskSheet.added(
                            projectField.text,
                            taskField.text,
                            noteField.text,
                            timeField.text
                        );
                    }
                    else {
                        addEditTaskSheet.edited(
                            index,
                            projectField.text,
                            taskField.text,
                            noteField.text,
                            timeField.text
                        );
                    }
                    addEditTaskSheet.close();
                }
            }
            Controls.Button {
                id: removeButton
                Layout.fillWidth: true
                flat: true
                text: i18nc("@action:button", "Remove")
                visible: mode === "edit"
                onClicked: {
                    addEditTaskSheet.removed(index)
                    addEditTaskSheet.close();
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
                    addEditTaskSheet.close();
                }
            }
            Controls.Button {
                id: cancelButton
                Layout.fillWidth: true
                flat: true
                text: i18nc("@action:button", "Cancel")
                onClicked: {
                    addEditTaskSheet.close();
                    projectField.currentIndex=0;
                    taskField.currentIndex = 0;
                    noteField.text = '';
                    timeField.text = '';
                }
            }
        }
}
