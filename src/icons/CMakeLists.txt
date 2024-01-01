# SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
set(icons
    22-apps-ruqola.png
    32-apps-ruqola.png
    48-apps-ruqola.png
    128-apps-ruqola.png
)

set(all_icons ${icons})

ecm_install_icons(ICONS ${all_icons}
    DESTINATION ${KDE_INSTALL_ICONDIR}
    THEME hicolor
)

