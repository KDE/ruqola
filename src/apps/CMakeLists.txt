# SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
include(ECMAddAppIcon)

add_executable(ruqola main.cpp)

target_link_libraries(ruqola
    libruqolacore
    libruqolawidgets
    librocketchatrestapi-qt
    KF${KF_MAJOR_VERSION}::Crash
    KF${KF_MAJOR_VERSION}::XmlGui
    KDAB::kdsingleapplication
)

if (NOT WIN32 AND NOT APPLE)

    if (QT_MAJOR_VERSION STREQUAL "5")
        target_link_libraries(ruqola Qt::X11Extras)
    else()
        target_link_libraries(ruqola Qt::GuiPrivate)
    endif()
endif()

file(GLOB_RECURSE RUQOLA_ICONS "${PROJECT_SOURCE_DIR}/src/icons/*-apps-ruqola.png")
ecm_add_app_icon(appIcons ICONS "${RUQOLA_ICONS}" )
target_sources(ruqola PRIVATE ${appIcons})

install(TARGETS ruqola ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
