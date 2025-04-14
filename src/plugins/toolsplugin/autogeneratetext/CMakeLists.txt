# SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
if (0)
kcoreaddons_add_plugin(ruqola_autogenerateplugin INSTALL_NAMESPACE ruqolaplugins/toolsplugins)

ecm_qt_declare_logging_category(ruqola_autogenerateplugin HEADER autogenerateplugin_debug.h
    IDENTIFIER RUQOLA_AUTOGENERATETEXT_PLUGIN_LOG CATEGORY_NAME org.kde.ruqola_grabscreenplugun
    DESCRIPTION "ruqola (autogenerate plugin)" EXPORT RUQOLA)

kconfig_add_kcfg_files(ruqola_autogenerateplugin autogenerateplugintoolconfig.kcfgc)

target_sources(ruqola_autogenerateplugin PRIVATE
)
target_link_libraries(ruqola_autogenerateplugin
    libruqolawidgets
    librocketchatrestapi-qt
)
endif()
