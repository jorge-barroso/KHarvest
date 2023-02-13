// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

#ifndef KHARVEST_TRAYICON_SNI_H
#define KHARVEST_TRAYICON_SNI_H


#include <KStatusNotifierItem>

class TrayIcon : public KStatusNotifierItem {
Q_OBJECT
public:
    explicit TrayIcon(QObject *parent = nullptr);

    void show();

signals:

    void showWindow();
};


#endif //KHARVEST_TRAYICON_SNI_H
