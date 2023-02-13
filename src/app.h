// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

#pragma once

#include <QObject>

#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
#include "trayicon.h"
#elif !defined(Q_OS_ANDROID)
#include "trayicon_sni.h"
#endif

#include "windowcontroller.h"

class QWindow;

class App : public QObject {
Q_OBJECT

public:
    explicit App(QObject *parent = nullptr);

signals:

    // Logout
    void logout();

private:
    TrayIcon trayIcon;
    WindowController &windowController;
};
