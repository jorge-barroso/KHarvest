// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

#ifndef KHARVEST_WINDOWCONTROLLER_H
#define KHARVEST_WINDOWCONTROLLER_H


#include <QObject>
#include <QWindow>

class WindowController : public QObject
{
Q_OBJECT

public:
    static WindowController &instance();

    void setWindow(QWindow *window);

    // Restore current window geometry
    Q_INVOKABLE static void restoreWindowGeometry(QWindow *window, const QString &group = QStringLiteral("main")) ;
    // Save current window geometry
    Q_INVOKABLE static void saveWindowGeometry(QWindow *window, const QString &group = QStringLiteral("main")) ;

public slots:
    void showAndRaiseWindow(const QString &startupId = QString());

private:
    WindowController() = default;

    QWindow *mWindow = nullptr;
};


#endif //KHARVEST_WINDOWCONTROLLER_H
