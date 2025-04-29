# SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause


kcoreaddons_add_plugin(ruqola_aitextplugin INSTALL_NAMESPACE ruqolaplugins/textplugins)

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
)

target_link_libraries(ruqola_aitextplugin
    libruqolawidgets
    KF6::TextAutogenerateText
)
