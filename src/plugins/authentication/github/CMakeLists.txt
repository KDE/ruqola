# SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

set(ruqola_githubauthenticationplugin_SRCS
    githubauthenticationinterface.cpp
    githubpluginauthentication.cpp
    githubauthenticationinterface.h
    githubpluginauthentication.h
    githubauthenticationjob.h
    githubauthenticationjob.cpp
    githubauthenticationsettings.h
    githubauthenticationsettings.cpp
)

ecm_qt_declare_logging_category(ruqola_githubauthenticationplugin_SRCS HEADER githubauthenticationplugin_debug.h
    IDENTIFIER RUQOLA_GITHUBAUTHENTICATION_PLUGIN_LOG CATEGORY_NAME org.kde.ruqola_githubauthenticationplugin
    DESCRIPTION "ruqola (github authentication plugin)" EXPORT RUQOLA)

kcoreaddons_add_plugin(ruqola_githubauthenticationplugin SOURCES
    ${ruqola_githubauthenticationplugin_SRCS} INSTALL_NAMESPACE ruqolaplugins/authentication)

target_link_libraries(ruqola_githubauthenticationplugin
    libruqolacore
)
if (QT_MAJOR_VERSION STREQUAL "6")
    target_link_libraries(ruqola_githubauthenticationplugin qt6keychain)
else()
    target_link_libraries(ruqola_githubauthenticationplugin qt5keychain)
endif()

if (BUILD_TESTING)
    add_subdirectory(autotests)
    add_subdirectory(tests)
endif()

