// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami

Kirigami.ScrollablePage {
    id: loginPage
    title: i18n("Login")

    leftPadding: Kirigami.Units.SmallSpacing
    rightPadding: Kirigami.Units.SmallSpacing

    background: Rectangle {
        color: Kirigami.Theme.backgroundColor
        Kirigami.Theme.colorSet: Kirigami.Theme.Window
        Image {
            id: backgroundImage
            height: parent.height*3/8
            sourceSize: Qt.size(width, height)
            source: "qrc:/icons/outline-logo.svg"
            fillMode: Image.PreserveAspectFit
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
        }
    }

    ColumnLayout {
        width: parent.width
        height: parent.height
        Kirigami.AbstractCard {
            Layout.fillHeight: false
            header: Kirigami.Heading {
                text: i18nc("@header:login", "Login")
                level: 2
            }
            contentItem: Controls.Label {
                wrapMode: Text.WordWrap
                text: i18nc("@label:login", "Please complete logging in from your browser, the application will automatically load your data once you've complete this step")
            }
        }
    }
}