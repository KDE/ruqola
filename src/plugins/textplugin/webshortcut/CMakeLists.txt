# SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

kcoreaddons_add_plugin(ruqola_webshortcuttextplugin INSTALL_NAMESPACE ruqolaplugins/textplugins)

target_sources(ruqola_webshortcuttextplugin PRIVATE
    webshortcuttextinterface.cpp
    webshortcuttextplugin.cpp
    webshortcuttextinterface.h
    webshortcuttextplugin.h
)
target_link_libraries(ruqola_webshortcuttextplugin
    libruqolacore libruqolawidgets
)
if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()

