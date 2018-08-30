
add_subdirectory(restapi)
add_subdirectory(ruqolacore)
add_subdirectory(plugins)
add_subdirectory(icons)
add_subdirectory(apps)

install(PROGRAMS data/org.kde.ruqola.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES data/org.kde.ruqola.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

