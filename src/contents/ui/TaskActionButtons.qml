// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kharvest 1.0 as KHarvest

GridLayout {
    id: actionButtonsLayout
    rowSpacing: Kirigami.Units.smallSpacing
    columnSpacing: Kirigami.Units.smallSpacing
    columns: 2

    // START/STOP TASK
    Controls.Button {
        id: startStopButton
        checkable: true
        checked: model.started
        Layout.alignment: Qt.AlignRight
        Layout.columnSpan: 1
        icon.name: model.started ? "media-playback-stop" : "media-playback-start"
        onClicked: {
            if(checked) {
                console.info('starting new task');
                model.started = true;
                startCurrent();
            } else {
                stopCurrent();
            }
        }
    }

    // SAVE/REMOVE FAVOURITE TASK
    Controls.Button {
        id: favUnfavButton
        Layout.alignment: Qt.AlignRight
        Layout.columnSpan: 1
        icon.name: "bookmark-new"
        // onClicked: {}
    }

    // EDIT TASK
    Controls.Button {
        id: editButton
        Layout.alignment: Qt.AlignRight
        Layout.columnSpan: 1
        icon.name: "document-edit"
        onClicked: openEditTaskSheet(index, model)
    }

    // REMOVE TASK
    Controls.Button {
        id: removeButton
        Layout.alignment: Qt.AlignRight
        Layout.columnSpan: 1
        icon.name: "list-remove"
        onClicked: addedTasksList.taskRemoved(index)
    }
}
