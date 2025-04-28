# SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause


kcoreaddons_add_plugin(ruqola_aitextplugin INSTALL_NAMESPACE ruqolaplugins/textplugins)

target_sources(ruqola_aitextplugin PRIVATE
)
target_link_libraries(ruqola_aitextplugin
    libruqolawidgets
)
