set(icons
    22-apps-ruqola.png
    32-apps-ruqola.png
    48-apps-ruqola.png
)

set(all_icons ${icons})

ecm_install_icons(ICONS ${all_icons}
    DESTINATION ${KDE_INSTALL_ICONDIR}
    THEME hicolor
)

