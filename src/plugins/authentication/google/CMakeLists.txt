set(ruqola_googleauthenticationplugin_SRCS
    googleauthenticationinterface.cpp
    googlepluginauthentication.cpp
    googlejob.cpp
    )

ecm_qt_declare_logging_category(ruqola_googleauthenticationplugin_SRCS HEADER googleauthenticationplugin_debug.h
    IDENTIFIER RUQOLA_GOOGLEAUTHENTICATION_PLUGIN_LOG CATEGORY_NAME org.kde.ruqola_googleauthenticationplugin)
qt5_add_resources(ruqola_googleauthenticationplugin_SRCS googleauthentication.qrc)

kcoreaddons_add_plugin(ruqola_googleauthenticationplugin JSON ruqola_googleauthenticationplugin.json SOURCES
    ${ruqola_googleauthenticationplugin_SRCS} INSTALL_NAMESPACE ruqolaplugins/authentication)

target_link_libraries(ruqola_googleauthenticationplugin
    libruqolacore ${o2_LIBRARIES}
    )

if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()
