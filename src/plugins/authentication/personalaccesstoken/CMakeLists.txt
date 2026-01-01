# SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

set(ruqola_personalaccesstokenauthenticationplugin_SRCS
    personalaccesstokenauthenticationinterface.cpp
    personalaccesstokenpluginauthentication.cpp
    personalaccesstokenauthenticationinterface.h
    personalaccesstokenpluginauthentication.h
    personalaccesstokenauthenticationconfigwidget.h
    personalaccesstokenauthenticationconfigwidget.cpp
)

ecm_qt_declare_logging_category(ruqola_personalaccesstokenauthenticationplugin_SRCS HEADER personalaccesstokenauthenticationplugin_debug.h
    IDENTIFIER RUQOLA_PERSONALACCESSTOKENAUTHENTICATION_PLUGIN_LOG CATEGORY_NAME org.kde.ruqola_personalaccesstokenauthenticationplugin
    DESCRIPTION "ruqola (personalaccesstoken authentication plugin)" EXPORT RUQOLA
)

kcoreaddons_add_plugin(ruqola_personalaccesstokenauthenticationplugin SOURCES
    ${ruqola_personalaccesstokenauthenticationplugin_SRCS} INSTALL_NAMESPACE ruqolaplugins/authentication
)

target_link_libraries(
    ruqola_personalaccesstokenauthenticationplugin
    libruqolacore
    qt6keychain
    KF6::WidgetsAddons
)

if(BUILD_TESTING)
    add_subdirectory(autotests)
    add_subdirectory(tests)
endif()
