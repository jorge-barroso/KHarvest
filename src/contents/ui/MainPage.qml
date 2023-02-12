// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kharvest 1.0 as KHarvest

Kirigami.ScrollablePage {
    id: page
    title: i18n("Tasks")

    Layout.fillWidth: true

    actions {
        main: Kirigami.Action {
            id: addTaskAction
            icon.name: "list-add"
            text: i18nc("@action:button", "Add Task")
            tooltip: i18n("Add a New Task")
            onTriggered: openAddTaskSheet()
        }
        contextualActions: [
            Kirigami.Action {
                text: i18nc("@action:button", "Favourites")
                icon.name: "bookmarks"
                onTriggered: applicationWindow().pageStack.layers.push("qrc:/FavouritesPage.qml")
            }
        ]
    }


    header: ColumnLayout {
                id: dateToolBar
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Kirigami.ActionToolBar {
                    alignment: Qt.AlignCenter
                    flat: true
                    actions: [
                       Kirigami.Action {
                           icon.name: "go-previous"
                           onTriggered: {
                                KHarvest.AppDate.addDays(-1);
                           }
                       },
                       Kirigami.Action {
                           icon.name: "view-calendar"
                           onTriggered: {
                                KHarvest.AppDate.date = new Date();
                           }
                       },
                       Kirigami.Action {
                           icon.name: "go-next"
                           onTriggered: {
                                KHarvest.AppDate.addDays(1);
                           }
                       }
                   ]
                }
                Kirigami.Separator {
                    Layout.fillWidth: true
                    visible: true
                    color: 'transparent'
                }
                Controls.Label {
                    id: dateLabel
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    font.bold: true
                    text: KHarvest.AppDate.date.toLocaleDateString()
                }
            }


    Kirigami.CardsListView {
        id: layout
        model: KHarvest.AddedTasksModel {
            list: addedTasksList
        }
        delegate: TaskDelegate {
            id: taskDelegate
        }
    }

}
