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
        harvesthandler.cpp
        harvesthandler.h
        harvestproject.h
        harvesttask.h
        main.cpp
        maputils.h
        projectslist.cpp
        projectslist.h
        projectsmodel.cpp
        projectsmodel.h
        settingsmanager.cpp
        settingsmanager.h
        task.cpp
        task.h
        tasksmodel.cpp
        tasksmodel.h
        resources.qrc)

target_link_libraries(kharvest
        Qt::Core
        Qt::Gui
        Qt::Qml
        Qt::Quick
        Qt::QuickControls2
        Qt::Svg
        Qt::Network
        KF5::I18n
        KF5::CoreAddons
        KF5::ConfigCore
        KF5::ConfigGui)

if (ANDROID)
    kirigami_package_breeze_icons(ICONS
            list-add
            help-about
            application-exit
            applications-graphics
            )
endif ()

kconfig_add_kcfg_files(kharvest GENERATE_MOC kharvestconfig.kcfgc)
install(TARGETS kharvest ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
