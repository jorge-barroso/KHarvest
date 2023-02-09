// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

import QtQuick 2.6
import QtQuick.Controls 2.0 as Controls
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.13 as Kirigami
import org.kde.kharvest 1.0

Kirigami.AbstractCard {
    id: taskDelegate

    contentItem: Item {
        // implicitWidth/Height define the natural width/height of an item if no width or height is specified.
        // The setting below defines a component's preferred size based on its content
        implicitWidth: delegateLayout.implicitWidth
        implicitHeight: delegateLayout.implicitHeight

        GridLayout {
            id: delegateLayout
            anchors {
                left: parent.left
                top: parent.top
                right: parent.right
            }
            rowSpacing: Kirigami.Units.largeSpacing
            columnSpacing: Kirigami.Units.largeSpacing
            columns: 2

            ColumnLayout {
                Kirigami.Heading {
                    Layout.fillWidth: true
                    level: 1
                    text: projectName
                }
                Kirigami.Separator {
                    Layout.fillWidth: true
                    visible: false
                }
                Controls.Label {
                    Layout.fillWidth: true
                    wrapMode: Text.WordWrap
                    text: taskName
                    visible: taskName.length > 0
                }
            }



            Controls.Button {
                id: unfavButton
                Layout.alignment: Qt.AlignRight
                Layout.columnSpan: 1
                icon.name: "bookmark-remove"
                // onClicked: showPassiveNotification(i18n("Unfav clicked"))
                onClicked: unfavTask(index)
            }
        }
    }
}
