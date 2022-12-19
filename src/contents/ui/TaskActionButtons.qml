// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: %{CURRENT_YEAR} %{AUTHOR} <%{EMAIL}>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kharvest 1.0

GridLayout {
    id: actionButtonsLayout
    rowSpacing: Kirigami.Units.smallSpacing
    columnSpacing: Kirigami.Units.smallSpacing
    columns: 2

    /*media-plaback-start and media-playback-stop*/
    Controls.Button {
        id: startStopButton
        Layout.alignment: Qt.AlignRight
        Layout.columnSpan: 1
        icon.name: "media-playback-start"
        // onClicked: {}
    }

    /*bookmark-new and bookmark-remove*/
    Controls.Button {
        id: favUnfavButton
        Layout.alignment: Qt.AlignRight
        Layout.columnSpan: 1
        icon.name: "bookmark-new"
        // onClicked: {}
    }

    Controls.Button {
        id: editButton
        Layout.alignment: Qt.AlignRight
        Layout.columnSpan: 1
        icon.name: "document-edit"
        onClicked: openPopulateSheet("edit")
    }

    Controls.Button {
        id: removeButton
        Layout.alignment: Qt.AlignRight
        Layout.columnSpan: 1
        icon.name: "list-remove"
        onClicked: deleteTask(index)
    }
}
