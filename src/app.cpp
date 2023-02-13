// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

#include "app.h"

#include <KWindowConfig>

#include <QQuickWindow>
#include <QGuiApplication>

App::App(QObject *parent)
    : QObject(parent)
    , trayIcon(this)
    , windowController{WindowController::instance()} {
    trayIcon.show();
    connect(&trayIcon, &TrayIcon::showWindow, &windowController, [this]() {
        windowController.showAndRaiseWindow(QString());
    });
    QGuiApplication::setQuitOnLastWindowClosed(false);
}
