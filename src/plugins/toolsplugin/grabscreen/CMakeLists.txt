# SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

kcoreaddons_add_plugin(ruqola_grabscreenplugin INSTALL_NAMESPACE ruqolaplugins/toolsplugins)

ecm_qt_declare_logging_category(ruqola_grabscreenplugin HEADER grabscreenplugin_debug.h
    IDENTIFIER RUQOLA_GRABSCREEN_PLUGIN_LOG CATEGORY_NAME org.kde.ruqola_grabscreenplugin
    DESCRIPTION "ruqola (grab screen plugin)" EXPORT RUQOLA)

kconfig_add_kcfg_files(ruqola_grabscreenplugin grabscreenplugintoolconfig.kcfgc)

target_sources(ruqola_grabscreenplugin PRIVATE
    grabscreenplugintool.h
    grabscreenplugintool.cpp
    grabscreenplugintoolinterface.h
    grabscreenplugintoolinterface.cpp
    grabscreenpluginjob.h
    grabscreenpluginjob.cpp
    grabscreenplugintoolutil.h
    grabscreenplugintoolutil.cpp
    grabscreenplugintoolconfigurewidget.h
    grabscreenplugintoolconfigurewidget.cpp
    grabscreenplugintoolconfiguredialog.h
    grabscreenplugintoolconfiguredialog.cpp
)
target_link_libraries(ruqola_grabscreenplugin
    libruqolawidgets
    librocketchatrestapi-qt
)
