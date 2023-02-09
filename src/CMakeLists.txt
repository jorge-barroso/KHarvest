# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: %{CURRENT_YEAR} %{AUTHOR} <%{EMAIL}>

add_executable(kharvest
    about.cpp
    about.h
    app.cpp
    app.h
    # harvesthandler.cpp
    # harvesthandler.h
    harvestproject.h
    harvesttask.h
    main.cpp
    projectslist.h
    projectslist.cpp
    projectsmodel.cpp
    projectsmodel.h
    settingsmanager.cpp
    settingsmanager.h
    task.h
    resources.qrc tasksmodel.h tasksmodel.cpp)

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
endif()

kconfig_add_kcfg_files(kharvest GENERATE_MOC kharvestconfig.kcfgc)
install(TARGETS kharvest ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
