// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

#ifndef KHARVEST_TRAYICON_H
#define KHARVEST_TRAYICON_H


#include <QSystemTrayIcon>

class TrayIcon : public QSystemTrayIcon
{
Q_OBJECT
public:
    explicit TrayIcon(QObject *parent = nullptr);

Q_SIGNALS:
    void showWindow();
};


#endif //KHARVEST_TRAYICON_H
