# SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause


kcoreaddons_add_plugin(ruqola_aitextplugin INSTALL_NAMESPACE ruqolaplugins/textplugins)

ecm_qt_declare_logging_category(ruqola_aitextplugin HEADER ruqola_aitextplugin_debug.h
    IDENTIFIER RUQOLA_AITEXT_PLUGIN_LOG CATEGORY_NAME org.kde.ruqola_aitextplugin
    DESCRIPTION "ruqola (ai text plugin)" EXPORT RUQOLA)

target_sources(ruqola_aitextplugin PRIVATE
    aitextinterface.cpp
    aitextinterface.h
    aitextplugin.cpp
    aitextplugin.h
    aitextmenuwidget.h
    aitextmenuwidget.cpp
    aitextmenuconfiguredialog.h
    aitextmenuconfiguredialog.cpp
    aitextmenuconfigurewidget.h
    aitextmenuconfigurewidget.cpp
    aitextinfo.h
    aitextinfo.cpp
    aitextmodel.h
    aitextmodel.cpp
    aitextmanager.h
    aitextmanager.cpp
    aitextlistview.h
    aitextlistview.cpp
    aitextlistviewdelegate.h
    aitextlistviewdelegate.cpp
)

target_link_libraries(ruqola_aitextplugin
    libruqolawidgets
    KF6::TextAutoGenerateText
)
