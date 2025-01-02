# SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

kcoreaddons_add_plugin(ruqola_grabscreenplugin INSTALL_NAMESPACE ruqolaplugins/toolsplugins)

target_sources(ruqola_grabscreenplugin PRIVATE
)
target_link_libraries(ruqola_grabscreenplugin
    libruqolawidgets
)
# if(BUILD_TESTING)
#     add_subdirectory(autotests)
# endif()
