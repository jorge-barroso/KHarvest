/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>
*/

#include <QQmlApplicationEngine>
#include <QUrl>
#include <QtQml>
#include <QQuickStyle>
#include <QStringLiteral>
#include <QObject>
#include <QQuickWindow>

#ifdef Q_OS_ANDROID
#include <QGuiApplication>
#else
#include <QApplication>
#endif

#include <KAboutData>
#include <KLocalizedContext>
#include <KLocalizedString>

#include "about.h"
#include "app.h"
#include "projectslist.h"
#include "projectsmodel.h"
#include "tasksmodel.h"
#include "addedtaskslist.h"
#include "addedtasksmodel.h"
#include "version-kharvest.h"
#include "kharvestconfig.h"
#include "harvesthandler.h"
#include "tasksmanager.h"
#include "favouriteslist.h"
#include "favouritesmodel.h"
#include "appdate.h"

static QQuickWindow* windowFromEngine(QQmlApplicationEngine *engine)
{
    const auto rootObjects = engine->rootObjects();
    auto *window(qobject_cast<QQuickWindow *>(rootObjects.first()));
    Q_ASSERT(window);
    return window;
}

Q_DECL_EXPORT int main(int argc, char *argv[]) {
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
    qmlRegisterType<AddedTasksModel>("org.kde.kharvest", 1, 0, "AddedTasksModel");
    qmlRegisterType<FavouritesModel>("org.kde.kharvest", 1, 0, "FavouritesModel");
    qmlRegisterUncreatableType<ProjectsList>("org.kde.kharvest", 1, 0, "ProjectsList",
                                             QStringLiteral("ProjectsList should not be created in QML"));
    qmlRegisterUncreatableType<AddedTasksList>("org.kde.kharvest", 1, 0, "AddedTasksList",
                                               QStringLiteral("AddedTasksList should not be created in QML"));
    qmlRegisterUncreatableType<FavouritesList>("org.kde.kharvest", 1, 0, "FavouritesList",
                                               QStringLiteral("FavouritesList should not be created in QML"));

    AppDate appDate;
    qmlRegisterSingletonInstance("org.kde.kharvest", 1, 0, "AppDate", &appDate);
    qmlRegisterSingletonInstance("org.kde.kharvest", 1, 0, "HarvestHandler", HarvestHandler::instance().get());

    QQmlApplicationEngine engine;

    auto config = KHarvestConfig::self();
    qmlRegisterSingletonInstance("org.kde.kharvest", 1, 0, "Config", config);

    AboutType about;
    qmlRegisterSingletonInstance("org.kde.kharvest", 1, 0, "AboutType", &about);

    App application;
    qmlRegisterSingletonInstance("org.kde.kharvest", 1, 0, "App", &application);

    qmlRegisterSingletonInstance("org.kde.kharvest", 1, 0, "WindowController", &WindowController::instance());

    ProjectsList projectsList;
    engine.rootContext()->setContextProperty(QStringLiteral("projectsList"), &projectsList);
    AddedTasksList addedTasksList(&appDate);
    engine.rootContext()->setContextProperty(QStringLiteral("addedTasksList"), &addedTasksList);
    FavouritesList favouritesList;
    engine.rootContext()->setContextProperty(QStringLiteral("favouritesList"), &favouritesList);
    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    QWindow *window = windowFromEngine(&engine);
    WindowController::instance().setWindow(window);

    TasksManager tasksManager(nullptr, projectsList, addedTasksList, favouritesList);
    qmlRegisterSingletonInstance("org.kde.kharvest", 1, 0, "TasksManager", &tasksManager);

    QObject::connect(&application, &App::logout, HarvestHandler::instance().get(), &HarvestHandler::logout_cleanup);
    QObject::connect(&appDate, &AppDate::dateChanged, &addedTasksList, &AddedTasksList::appDateChanged);

    return app.exec();
}
