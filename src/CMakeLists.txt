# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

add_executable(kharvest
        about.cpp
        about.h
        addedtasksmodel.cpp
        addedtasksmodel.h
        addedtaskslist.cpp
        addedtaskslist.h
        app.cpp
        app.h
        appdate.cpp
        appdate.h
        favouriteslist.cpp
        favouriteslist.h
        favouritesmodel.cpp
        favouritesmodel.h
        harvesthandler.cpp
        harvesthandler.h
        harvestproject.h
        harvesttask.h
        keychain.cpp
        keychain.h
        main.cpp
        maputils.h
        projectslist.cpp
        projectslist.h
        projectsmodel.cpp
        projectsmodel.h
        task.cpp
        task.h
        tasksmanager.cpp
        tasksmanager.h
        tasksmodel.cpp
        tasksmodel.h
        windowcontroller.cpp
        windowcontroller.h
        resources.qrc)

target_link_libraries(kharvest PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Qml
        Qt::Quick
        Qt::QuickControls2
        Qt::Svg
        Qt::Network
        KF5::Kirigami2
        KF5::Notifications
        KF5::I18n
        KF5::CoreAddons
        KF5::ConfigCore
        KF5::ConfigGui
        ${QTKEYCHAIN_LIBRARIES})

if (NOT ANDROID)
    if (NOT WIN32 AND NOT APPLE)
        target_sources(kharvest PRIVATE trayicon_sni.h trayicon_sni.cpp)
    else ()
        target_sources(kharvest PRIVATE trayicon.h trayicon.cpp)
    endif ()
    target_link_libraries(kharvest PUBLIC KF5::WindowSystem)
    target_compile_definitions(kharvest PUBLIC -DHAVE_WINDOWSYSTEM)
else ()
    kirigami_package_breeze_icons(ICONS
            list-add
            help-about
            application-exit
            applications-graphics
            )
endif ()

kconfig_add_kcfg_files(kharvest GENERATE_MOC kharvestconfig.kcfgc)
install(TARGETS kharvest ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
