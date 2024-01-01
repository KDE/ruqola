# SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

set(ruqola_gitlabauthenticationplugin_SRCS
    gitlabauthenticationinterface.cpp
    gitlabpluginauthentication.cpp
    gitlabauthenticationinterface.h
    gitlabpluginauthentication.h
    gitlabauthenticationjob.h
    gitlabauthenticationjob.cpp
)

ecm_qt_declare_logging_category(ruqola_gitlabauthenticationplugin_SRCS HEADER gitlabauthenticationplugin_debug.h
    IDENTIFIER RUQOLA_GITLABAUTHENTICATION_PLUGIN_LOG CATEGORY_NAME org.kde.ruqola_gitlabauthenticationplugin
    DESCRIPTION "ruqola (gitlab authentication plugin)" EXPORT RUQOLA)

kcoreaddons_add_plugin(ruqola_gitlabauthenticationplugin SOURCES
    ${ruqola_gitlabauthenticationplugin_SRCS} INSTALL_NAMESPACE ruqolaplugins/authentication)

target_link_libraries(ruqola_gitlabauthenticationplugin
    libruqolacore
)
if (QT_MAJOR_VERSION STREQUAL "6")
    target_link_libraries(ruqola_gitlabauthenticationplugin qt6keychain)
else()
    target_link_libraries(ruqola_gitlabauthenticationplugin qt5keychain)
endif()

if (BUILD_TESTING)
    add_subdirectory(autotests)
    add_subdirectory(tests)
endif()

