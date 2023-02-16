// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

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
