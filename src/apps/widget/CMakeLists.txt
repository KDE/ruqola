# SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
include(ECMAddAppIcon)

add_executable(ruqola main.cpp)

target_link_libraries(ruqola
    libruqolacore
    libruqolawidgets
    librocketchatrestapi-qt5
    KF5::Crash
    KF5::DBusAddons
    KF5::XmlGui
)

file(GLOB_RECURSE RUQOLA_ICONS "${PROJECT_SOURCE_DIR}/src/icons/*-apps-ruqola.png")
ecm_add_app_icon(appIcons ICONS "${RUQOLA_ICONS}" )
target_sources(ruqola PRIVATE ${appIcons})

install(TARGETS ruqola ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
