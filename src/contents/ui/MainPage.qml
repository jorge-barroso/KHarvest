// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: %{CURRENT_YEAR} %{AUTHOR} <%{EMAIL}>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kharvest 1.0

Kirigami.ScrollablePage {
    id: page

    Layout.fillWidth: true

    title: i18n("Tasks")

    actions {
        main: Kirigami.Action {
            id: addTaskAction
            icon.name: "list-add"
            text: i18nc("@action:button", "Add Task")
            tooltip: i18n("Add a New Task")
            onTriggered: openPopulateSheet("add")
        }
        contextualActions: [
            Kirigami.Action {
                text: i18nc("@action:button", "Favourites")
                icon.name: "bookmarks"
                onTriggered: applicationWindow().pageStack.layers.push("qrc:/FavouritesPage.qml")
            }
        ]
    }

    Kirigami.CardsListView {
        id: layout
        model: taskModel
        delegate: TaskDelegate {}
    }
}
