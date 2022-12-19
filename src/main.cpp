/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: %{CURRENT_YEAR} %{AUTHOR} <%{EMAIL}>
*/

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QUrl>
#include <QtQml>

#include "about.h"
#include "app.h"
#include "version-kharvest.h"
#include <KAboutData>
#include <KLocalizedContext>
#include <KLocalizedString>

#include "kharvestconfig.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName(QByteArray("KDE"));
    QCoreApplication::setApplicationName(QByteArray("KHarvest"));

    KAboutData aboutData(
                         // The program name used internally.
                         QStringLiteral("KHarvest"),
                         // A displayable program name string.
                         i18nc("@title", "KHarvest"),
                         // The program version string.
                         QStringLiteral(KHARVEST_VERSION_STRING),
                         // Short description of what the app does.
                         i18n("Time tracking for Harvest Timer"),
                         // The license this code is released under.
                         KAboutLicense::GPL_V3,
                         // Copyright Statement.
                         "(c) 2022-" + QString::number(QDate::currentDate().year()));
    aboutData.setOrganizationDomain(QByteArray("kde.org"));
    aboutData.setProductName(QByteArray("KHarvest"));


    aboutData.addAuthor(i18nc("@info:credit", "Jorge Barroso (Baam)"),
                        i18nc("@info:credit", "Developer"),
                        QStringLiteral("jorge_barroso_11@hotmail.com"));

    aboutData.setHomepage(QStringLiteral("https://github.com/jorge-barroso/kharvest"));

    KAboutData::setApplicationData(aboutData);

    QQmlApplicationEngine engine;

    auto config = kharvestConfig::self();

    qmlRegisterSingletonInstance("org.kde.kharvest", 1, 0, "Config", config);

    AboutType about;
    qmlRegisterSingletonInstance("org.kde.kharvest", 1, 0, "AboutType", &about);

    App application;
    qmlRegisterSingletonInstance("org.kde.kharvest", 1, 0, "App", &application);

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
