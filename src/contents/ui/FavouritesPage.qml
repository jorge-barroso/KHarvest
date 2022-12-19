// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: %{CURRENT_YEAR} %{AUTHOR} <%{EMAIL}>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kharvest 1.0

Kirigami.ScrollablePage {
    id: favsPage

    Layout.fillWidth: true

    title: i18n("Favourites")

    actions {
        main: Kirigami.Action {
                id: addFavouriteAction
                icon.name: "bookmark-new"
                text: i18nc("@action:button", "Add Favourite")
                tooltip: i18n("Add a task to favourites")
                onTriggered: showPassiveNotification(i18n("Add fav clicked"))
            }
        contextualActions: [
            Kirigami.Action {
                id: closeFavouritesAction
                icon.name: "window-close"
                text: i18nc("@action:button", "Close Favourites")
                tooltip: i18n("Close Favourites Section")
                onTriggered: applicationWindow().pageStack.layers.pop()
            }
        ]
    }

    Kirigami.CardsListView {
        id: favouritesLayout
        model: favouritesModel
        delegate: FavouritesDelegate {}
    }
}

