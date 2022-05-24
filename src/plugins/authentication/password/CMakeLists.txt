# SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
set(ruqola_passwordauthenticationplugin_SRCS
    passwordauthenticationinterface.cpp
    passwordpluginauthentication.cpp
    passwordauthenticationinterface.h
    passwordpluginauthentication.h
)

ecm_qt_declare_logging_category(ruqola_passwordauthenticationplugin_SRCS HEADER passwordauthenticationplugin_debug.h
    IDENTIFIER RUQOLA_PASSWORDAUTHENTICATION_PLUGIN_LOG CATEGORY_NAME org.kde.ruqola_passwordauthenticationplugin
    DESCRIPTION "ruqola (password authentication plugin)" EXPORT RUQOLA)

kcoreaddons_add_plugin(ruqola_passwordauthenticationplugin SOURCES
    ${ruqola_passwordauthenticationplugin_SRCS} INSTALL_NAMESPACE ruqolaplugins/authentication)

target_link_libraries(ruqola_passwordauthenticationplugin
    libruqolacore
)

if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()
