// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

import QtQuick 2.6
import QtQuick.Controls 2.0 as Controls
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.13 as Kirigami
import org.kde.kharvest 1.0 as KHarvest

Kirigami.AbstractCard {
    id: favouritesDelegate

    showClickFeedback: true

    property var currentModel: {}

    contentItem: Item {
        id: cardItem
        // implicitWidth/Height define the natural width/height of an item if no width or height is specified.
        // The setting below defines a component's preferred size based on its content
        implicitWidth: favouriteRow.implicitWidth
        implicitHeight: favouriteRow.implicitHeight

        RowLayout {
            id: favouriteRow
            anchors.fill: parent
            Column {
                Layout.fillWidth: true
                Kirigami.Heading {
                    id: projectHeader
                    level: 1
                    text: model.header
                }
                Kirigami.Heading {
                    id: taskNameHeader
                    level: 3
                    wrapMode: Text.NoWrap
                    text: model.taskName
                    visible: taskName.length > 0
                }
            }

            Column {
                Controls.Button {
                    id: unfavButton

                    icon.name: "bookmark-remove"
                    onClicked: KHarvest.TasksManager.unfavouritedFromFavouritesPage(index)
                }
            }
            Component.onCompleted: {
                favouritesDelegate.currentModel = model
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                openAddTaskSheet(favouritesDelegate.currentModel)
            }
        }
    }
}
