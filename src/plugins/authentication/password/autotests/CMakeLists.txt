ecm_qt_declare_logging_category(ruqola_passwordauthenticationplugin_test_SRCS HEADER passwordauthenticationplugin_debug.h
    IDENTIFIER RUQOLA_PASSWORDAUTHENTICATION_PLUGIN_LOG CATEGORY_NAME org.kde.ruqola_passwordauthenticationplugin)

macro(add_ruqola_plugin_unittest _source _additional)
    set( _test ${ruqola_passwordauthenticationplugin_test_SRCS} ${_source}  ${_additional})
    get_filename_component(_name ${_source} NAME_WE)
    add_executable( ${_name} ${_test} )
    add_test(NAME ${_name} COMMAND ${_name} )

    ecm_mark_as_test(${_name})
    target_link_libraries( ${_name} Qt5::Test KF5::I18n libruqolacore)
endmacro ()

add_ruqola_plugin_unittest(passwordpluginauthenticationtest.cpp "../passwordpluginauthentication.cpp;../passwordauthenticationinterface.cpp")
add_ruqola_plugin_unittest(passwordauthenticationinterfacetest.cpp "../passwordauthenticationinterface.cpp")
