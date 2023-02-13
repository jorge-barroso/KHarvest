//
// Created by jorge on 13/02/23.
//

#include "trayicon_sni.h"
#include <KWindowSystem>

TrayIcon::TrayIcon(QObject *parent)
        : KStatusNotifierItem(parent)
{
    setCategory(KStatusNotifierItem::ItemCategory::ApplicationStatus);
    setIconByName("org.kde.kharvest.tray.light");
    connect(this, &KStatusNotifierItem::activateRequested, this, [this] {
        KWindowSystem::setCurrentXdgActivationToken(providedToken());
        emit showWindow();
    });
}

void TrayIcon::show()
{
    setStatus(Active);
}
