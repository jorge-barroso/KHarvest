//
// Created by jorge on 13/02/23.
//

#include "trayicon.h"

#include <QCoreApplication>
#include <QAction>
#include <QMenu>

#include <KLocalizedString>

TrayIcon::TrayIcon(QObject *parent)
    : QSystemTrayIcon(parent) {
//
    setIcon(QIcon("qrc:/icons/outline-logo.svg"));
    auto *menu = new QMenu();
    auto viewAction_ = new QAction(i18n("Show"), parent);

    connect(viewAction_, &QAction::triggered, this, &TrayIcon::showWindow);
    connect(this, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger) {
            Q_EMIT showWindow();
        }
    });

    menu->addAction(viewAction_);

    menu->addSeparator();

    auto quitAction = new QAction(i18n("Quit"), parent);
    quitAction->setIcon(QIcon::fromTheme("application-exit"));
    connect(quitAction, &QAction::triggered, QCoreApplication::instance(), QCoreApplication::quit);

    menu->addAction(quitAction);

    setContextMenu(menu);
}
