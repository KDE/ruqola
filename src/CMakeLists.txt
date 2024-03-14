# SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_subdirectory(rocketchatrestapi-qt)

add_subdirectory(core)

add_subdirectory(widgets)

add_subdirectory(plugins)

add_subdirectory(icons)

add_subdirectory(apps)

if(APPLE)
    set_target_properties(ruqola PROPERTIES
        MACOSX_BUNDLE_DISPLAY_NAME "Ruqola"
        MACOSX_BUNDLE_BUNDLE_NAME "Ruqola"
        MACOSX_BUNDLE_LONG_VERSION_STRING "Ruqola ${RELEASE_SERVICE_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}"
        MACOSX_BUNDLE_BUNDLE_VERSION "${RELEASE_SERVICE_VERSION}"
        MACOSX_BUNDLE_GUI_IDENTIFIER "org.kde.ruqola"
        MACOSX_BUNDLE_COPYRIGHT "2020-2024 The Ruqola Developers")
endif()

install(PROGRAMS data/org.kde.ruqola.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES data/org.kde.ruqola.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
install(FILES data/ruqola.notifyrc DESTINATION ${KDE_INSTALL_KNOTIFYRCDIR} )
install(FILES data/ruqola.openurl DESTINATION ${KDE_INSTALL_DATADIR}/messageviewer/openurlwith)
