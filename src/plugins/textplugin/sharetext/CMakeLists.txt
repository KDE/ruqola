# SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

kcoreaddons_add_plugin(ruqola_sharetextplugin INSTALL_NAMESPACE ruqolaplugins/textplugins)

target_sources(ruqola_sharetextplugin PRIVATE
    sharetextinterface.cpp
    sharetextplugin.cpp
    sharetextinterface.h
    sharetextplugin.h
    purposemenuwidget.cpp
    purposemenuwidget.h
)
target_link_libraries(ruqola_sharetextplugin
    libruqolacore libruqolawidgets
    KF${KF_MAJOR_VERSION}::Purpose
    KF${KF_MAJOR_VERSION}::PurposeWidgets
)

