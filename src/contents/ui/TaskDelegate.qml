// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

import QtQuick 2.6
import QtQuick.Controls 2.0 as Controls
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.13 as Kirigami
import org.kde.kharvest 1.0

Kirigami.AbstractCard {
    id: taskDelegate

    property int secondsToUpdate: 60
    property int baseUnitToOneSecond: 1000
    property int timeBetweenUpdates: secondsToUpdate * baseUnitToOneSecond

    function startCurrent() {
        console.info(root.runningTask);
        if(root.runningTask !== undefined) {
            root.runningTask.started = false;
        }

        root.runningTask = model;
    }

    function stopCurrent() {
        model.started = false;
        root.runningTask = undefined;
    }

    function increaseTimeCounter() {
        model.time = new Date(model.time.getTime() + timeBetweenUpdates);
    }

    Timer {
        id: taskTimer
        interval: timeBetweenUpdates; running: model.started; repeat: true;
        onTriggered: taskDelegate.increaseTimeCounter()
    }

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
            columns: 3

            ColumnLayout {
                Kirigami.Heading {
                    Layout.fillWidth: true
                    level: 1
                    text: model.header
                }
                Kirigami.Separator {
                    Layout.fillWidth: true
                    visible: false
                }
                Controls.Label {
                    Layout.fillWidth: true
                    wrapMode: Text.WordWrap
                    text: model.subtitle
                    visible: subtitle.length > 0
                }
            }

            Controls.Label {
                Layout.fillHeight: true
                text: model.timeLabel
            }

            TaskActionButtons {
                id: actionButtonsLayout
            }
        }

        Component.onCompleted: {
            if(model.started) {
                startCurrent();
            }
        }
    }
}
