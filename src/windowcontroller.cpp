// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

#include <KWindowConfig>
#include <KConfig>
#include <QStandardPaths>
#include <KWindowSystem>
#include <KStartupInfo>
#include "windowcontroller.h"

WindowController &WindowController::instance()
{
    static WindowController instance;
    return instance;
}

void WindowController::restoreWindowGeometry(QWindow *window, const QString &group) {
    KConfig dataResource(QStringLiteral("data"), KConfig::SimpleConfig, QStandardPaths::AppDataLocation);
    KConfigGroup windowGroup(&dataResource, QStringLiteral("Window-") + group);
    KWindowConfig::restoreWindowSize(window, windowGroup);
    KWindowConfig::restoreWindowPosition(window, windowGroup);
}

void WindowController::saveWindowGeometry(QWindow *window, const QString &group) {
    KConfig dataResource(QStringLiteral("data"), KConfig::SimpleConfig, QStandardPaths::AppDataLocation);
    KConfigGroup windowGroup(&dataResource, QStringLiteral("Window-") + group);
    KWindowConfig::saveWindowPosition(window, windowGroup);
    KWindowConfig::saveWindowSize(window, windowGroup);
    dataResource.sync();
}

void WindowController::setWindow(QWindow *window) {
    mWindow = window;
}

void WindowController::showAndRaiseWindow(const QString &startupId) {
    if (!mWindow->isVisible()) {
        mWindow->show();
    }

#ifdef HAVE_WINDOWSYSTEM
    if (!startupId.isEmpty()) {
        if (KWindowSystem::isPlatformX11()) {
            KStartupInfo::setNewStartupId(mWindow, startupId.toUtf8());
        } else if (KWindowSystem::isPlatformWayland()) {
            KWindowSystem::setCurrentXdgActivationToken(startupId);
        }
    }

    KWindowSystem::activateWindow(mWindow);
#endif
}
