# SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

kcoreaddons_add_plugin(ruqola_grabscreenplugin INSTALL_NAMESPACE ruqolaplugins/toolsplugins)

kconfig_add_kcfg_files(ruqola_grabscreenplugin grabscreenplugintoolconfig.kcfgc)

target_sources(ruqola_grabscreenplugin PRIVATE
    grabscreenplugintool.h grabscreenplugintool.cpp
    grabscreenplugintoolinterface.h grabscreenplugintoolinterface.cpp
    grabscreenpluginjob.h grabscreenpluginjob.cpp
    grabscreenplugintoolutil.h grabscreenplugintoolutil.cpp
)
target_link_libraries(ruqola_grabscreenplugin
    libruqolawidgets
)
# if(BUILD_TESTING)
#     add_subdirectory(autotests)
# endif()
