# SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
if(OPTION_BUILD_PYTHON_BINDINGS)
   set(PYTHON_VERSION "3.7" CACHE STRING  "Use specific python version to build the project.")
   find_package(Python3 ${PYTHON_VERSION} REQUIRED COMPONENTS Interpreter Development)
   find_package(Shiboken2 REQUIRED)
   find_package(PySide2 ${Qt5Widgets_VERSION} EXACT REQUIRED)
   include(PySide2ModuleBuild)
endif()

# Remove it when we will split lib
add_subdirectory(pimcommonautocorrection)
add_subdirectory(pimcommontexttranslator)
if (KPIMTEXTEDIT_TEXT_TO_SPEECH)
    add_subdirectory(kpimtextedittexttospeech)
endif()
add_subdirectory(rocketchatrestapi-qt5)

add_subdirectory(core)

add_subdirectory(widgets)

add_subdirectory(plugins)

add_subdirectory(icons)

add_subdirectory(apps)

install(PROGRAMS data/org.kde.ruqola.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES data/org.kde.ruqola.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
install(FILES data/ruqola.notifyrc DESTINATION ${KDE_INSTALL_KNOTIFYRCDIR} )
