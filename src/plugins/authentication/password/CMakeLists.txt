set(ruqola_passwordauthenticationplugin_SRCS
    passwordauthenticationinterface.cpp
    passwordpluginauthentication.cpp
    )

ecm_qt_declare_logging_category(ruqola_passwordauthenticationplugin_SRCS HEADER passwordauthenticationplugin_debug.h
    IDENTIFIER RUQOLA_PASSWORDAUTHENTICATION_PLUGIN_LOG CATEGORY_NAME org.kde.ruqola_passwordauthenticationplugin)

kcoreaddons_add_plugin(ruqola_passwordauthenticationplugin JSON ruqola_passwordauthenticationplugin.json SOURCES
    ${ruqola_passwordauthenticationplugin_SRCS} INSTALL_NAMESPACE ruqolaplugins/authentication)

target_link_libraries(ruqola_passwordauthenticationplugin
    libruqolacore
    )

if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()
