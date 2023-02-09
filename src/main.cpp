/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>
*/

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QUrl>
#include <QtQml>
#include <QQuickStyle>
#include <QStringLiteral>

#include "about.h"
#include "app.h"
#include "projectslist.h"
#include "projectsmodel.h"
#include "tasksmodel.h"
#include "version-kharvest.h"
#include <KAboutData>
#include <KLocalizedContext>
#include <KLocalizedString>

#include "kharvestconfig.h"
#include "harvesthandler.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QNetworkProxyFactory::setUseSystemConfiguration(true);

#ifdef Q_OS_ANDROID
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle(QStringLiteral("org.kde.breeze"));
#else
    QApplication app(argc, argv);
    // Default to org.kde.desktop style unless the user forces another style
    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE")) {
        QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
    }
#endif

#ifdef Q_OS_WINDOWS
    if (AttachConsole(ATTACH_PARENT_PROCESS)) {
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
    }

    QApplication::setStyle(QStringLiteral("breeze"));
    auto font = app.font();
    font.setPointSize(10);
    app.setFont(font);
#endif

    KLocalizedString::setApplicationDomain("kharvest");

    QCoreApplication::setOrganizationName(QByteArray("KDE"));

    QCoreApplication::setApplicationName(QByteArray("KHarvest"));

    const QString currentYear{QString::number(QDate::currentDate().year())};
    KAboutData aboutData(
                         // The program name used internally.
                         QStringLiteral("kharvest"),
                         // A displayable program name string.
                         i18nc("@title", "KHarvest"),
                         // The program version string.
                         QStringLiteral(KHARVEST_VERSION_STRING),
                         // Short description of what the app does.
                         i18n("Time tracking for Harvest Timer"),
                         // The license this code is released under.
                         KAboutLicense::GPL_V3,
                         // Copyright Statement.
                         "© 2022-" + currentYear + " Jorge Barroso, 2022-"+currentYear+" KDE Community");
    aboutData.addAuthor(i18nc("@info:credit", "Jorge Barroso (Baam)"), i18nc("@info:credit", "Maintainer"), QStringLiteral("jorge_barroso_11@hotmail.com"));
    aboutData.setOrganizationDomain(QByteArray("kde.org"));
    aboutData.setBugAddress("https://bugs.kde.org/describecomponents.cgi?product=kharvest");
    aboutData.setProductName(QByteArray("KHarvest"));

    aboutData.setHomepage(QStringLiteral("https://github.com/jorge-barroso/kharvest"));

    KAboutData::setApplicationData(aboutData);

    qmlRegisterType<ProjectsModel>("org.kde.kharvest", 1, 0, "ProjectsModel");
    qmlRegisterType<TasksModel>("org.kde.kharvest", 1, 0, "TasksModel");
    qmlRegisterUncreatableType<ProjectsList>("org.kde.kharvest", 1, 0, "ProjectsList",
                                             QStringLiteral("ProjectsList should not be created in QML"));
    qmlRegisterSingletonInstance("org.kde.kharvest", 1, 0, "AccountManager", HarvestHandler::instance());

    QQmlApplicationEngine engine;

    auto config = KHarvestConfig::self();
    qmlRegisterSingletonInstance("org.kde.kharvest", 1, 0, "Config", config);

    AboutType about;
    qmlRegisterSingletonInstance("org.kde.kharvest", 1, 0, "AboutType", &about);

    App application;
    qmlRegisterSingletonInstance("org.kde.kharvest", 1, 0, "App", &application);

    ProjectsList projects_list;
    engine.rootContext()->setContextProperty(QStringLiteral("projectsList"), &projects_list);
    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
