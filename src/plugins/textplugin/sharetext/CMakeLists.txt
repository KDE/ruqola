# SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

kcoreaddons_add_plugin(ruqola_sharetextplugin INSTALL_NAMESPACE ruqolaplugins/textplugins)

ecm_qt_declare_logging_category(ruqola_sharetextplugin HEADER ruqola_sharetextplugin_debug.h
    IDENTIFIER RUQOLA_SHARETEXT_PLUGIN_LOG CATEGORY_NAME org.kde.ruqola_sharetextplugin
    DESCRIPTION "ruqola (share text plugin)" EXPORT RUQOLA
)

target_sources(
    ruqola_sharetextplugin
    PRIVATE
        sharetextinterface.cpp
        sharetextplugin.cpp
        sharetextinterface.h
        sharetextplugin.h
        purposemenuwidget.cpp
        purposemenuwidget.h
)
target_link_libraries(
    ruqola_sharetextplugin
    libruqolawidgets
    KF6::Purpose
    KF6::PurposeWidgets
)
