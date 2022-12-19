// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: %{CURRENT_YEAR} %{AUTHOR} <%{EMAIL}>

import QtQuick 2.9
import QtQuick.Controls 2.3 as Controls
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.19 as Kirigami

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
            // Kirigami.FormData.label: i18nc("@label:textbox", "Project:")
            textRole: "projectName"
            currentIndex: 0
            model: projectsModel
            onCurrentIndexChanged: console.debug(projectsModel.get(currentIndex).text + ", " + projectsModel.get(currentIndex).subitems)


        }
        Controls.ComboBox {
            id: taskField
            anchors {
                left: parent.left
                right: parent.right
            }
            Kirigami.FormData.label: i18nc("@label:textbox", "Task:")
            textRole: subitems
            model: projectsModel
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
            Kirigami.FormData.label: i1nc("@label:textbox", "Note:")
            text: mode === "add" ? "" : taskNote
        }

        Controls.TextField {
            id: timeField
            anchors.right: parent.right
            Kirigami.FormData.label: i1nc("@label:textbox", "Time:")
            inputMask: "00:00"
            placeholderText: i18n("HH:MM")
            text: mode === "add" ? "00:00" : timeTracked
        }

        RowLayout {
            anchors {
                left: parent.left
                right: parent.right
            }
            Controls.Button {
                id: doneButton
                Layout.fillWidth: true
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
                text: i18nc("@action:button", "Cancel")
                onClicked: {
                    addEditTaskSheet.close();
                    projectField.text = '';
                    taskField.text = '';
                    noteField.text = '';
                    timeField.text = '';
                }
            }
        }
    }
}
