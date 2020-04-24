set(ruqola_webshortcuttextplugin_SRCS
    webshortcuttextinterface.cpp
    webshortcuttextplugin.cpp
    )

kcoreaddons_add_plugin(ruqola_webshortcuttextplugin JSON ruqola_webshortcuttextplugin.json SOURCES
    ${ruqola_webshortcuttextplugin_SRCS} INSTALL_NAMESPACE ruqolaplugins/textplugins)

target_link_libraries(ruqola_webshortcuttextplugin
    libruqolacore libruqolawidgets
    )

