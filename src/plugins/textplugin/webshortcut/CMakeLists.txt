# SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
set(ruqola_webshortcuttextplugin_SRCS
    webshortcuttextinterface.cpp
    webshortcuttextplugin.cpp
    webshortcuttextinterface.h
    webshortcuttextplugin.h
    )

kcoreaddons_add_plugin(ruqola_webshortcuttextplugin JSON ruqola_webshortcuttextplugin.json SOURCES
    ${ruqola_webshortcuttextplugin_SRCS} INSTALL_NAMESPACE ruqolaplugins/textplugins)

target_link_libraries(ruqola_webshortcuttextplugin
    libruqolacore libruqolawidgets
    )
if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()

