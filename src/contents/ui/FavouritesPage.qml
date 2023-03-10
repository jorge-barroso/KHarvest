// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.20 as Kirigami
import org.kde.kharvest 1.0 as KHarvest

Kirigami.ScrollablePage {
    id: favouritesPage

    Layout.fillWidth: true

    title: i18n("Favourites")

    onBackRequested: addEditTaskSheet.parent = page
    Component.onCompleted: {
        addEditTaskSheet.parent = favouritesPage
    }

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
                onTriggered: {
                    addEditTaskSheet.parent = page
                    applicationWindow().pageStack.layers.pop()
                }
            }
        ]
    }

    Kirigami.CardsListView {
        id: favouritesLayout
        model: KHarvest.FavouritesModel {
            list: favouritesList
        }
        delegate: FavouritesDelegate {
            id: favouritesDelegate
        }
    }
}

