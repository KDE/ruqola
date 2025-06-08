# SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

kcoreaddons_add_plugin(ruqola_aiactionsplugin INSTALL_NAMESPACE ruqolaplugins/toolsplugins)

ecm_qt_declare_logging_category(ruqola_aiactionsplugin HEADER aiactionsplugin_debug.h
    IDENTIFIER RUQOLA_AUTOGENERATETEXT_AIACTIONS_LOG CATEGORY_NAME org.kde.ruqola_aiactions
    DESCRIPTION "ruqola (ai actions plugin)" EXPORT RUQOLA
)

target_sources(
    ruqola_aiactionsplugin
    PRIVATE
        aiactionsplugintoolinterface.cpp
        aiactionsplugintoolinterface.h
        aiactionsplugintool.cpp
        aiactionsplugintool.h
)
target_link_libraries(
    ruqola_aiactionsplugin
    libruqolawidgets
    KF6::TextAutoGenerateText
)
