
add_subdirectory(rocketchatrestapi-qt5)

add_subdirectory(core)

if (NOT ANDROID)
    add_subdirectory(widgets)
endif()

add_subdirectory(plugins)

add_subdirectory(icons)

add_subdirectory(apps)

install(PROGRAMS data/org.kde.ruqola.desktop data/org.kde.ruqolaqml.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES data/org.kde.ruqola.appdata.xml data/org.kde.ruqolaqml.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

