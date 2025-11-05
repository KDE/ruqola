# SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
include(ECMAddAppIcon)

add_executable(
    ruqola
    main.cpp
    ruqola.qrc
)

target_link_libraries(
    ruqola
    libruqolawidgets
    librocketchatrestapi-qt
    KF6::Crash
    KF6::XmlGui
    KF6::IconThemes
)
if(USE_DBUS)
    target_link_libraries(ruqola KF6::DBusAddons)
else()
    target_link_libraries(ruqola KDAB::kdsingleapplication)
    if(NOT WIN32 AND NOT APPLE)
        target_link_libraries(ruqola Qt::GuiPrivate)
    endif()
endif()

file(GLOB_RECURSE RUQOLA_ICONS "${PROJECT_SOURCE_DIR}/src/icons/*-apps-ruqola.png")
ecm_add_app_icon(appIcons ICONS "${RUQOLA_ICONS}")
target_sources(ruqola PRIVATE ${appIcons})

install(
    TARGETS
        ruqola
        ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)
