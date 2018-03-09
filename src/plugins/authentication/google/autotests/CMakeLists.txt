ecm_qt_declare_logging_category(ruqola_googleauthenticationplugin_test_SRCS HEADER googleauthenticationplugin_debug.h
    IDENTIFIER RUQOLA_GOOGLEAUTHENTICATION_PLUGIN_LOG CATEGORY_NAME org.kde.ruqola_googleauthenticationplugin)

macro(add_ruqola_plugin_unittest _source _additional)
    set( _test ${ruqola_googleauthenticationplugin_test_SRCS} ${_source}  ${_additional})
    get_filename_component(_name ${_source} NAME_WE)
    add_executable( ${_name} ${_test} )
    add_test(NAME ${_name} COMMAND ${_name} )

    ecm_mark_as_test(${_name})
    target_link_libraries( ${_name} Qt5::Test KF5::I18n libruqolacore ${o2_LIBRARIES})
endmacro ()

add_ruqola_plugin_unittest(googlepluginauthenticationtest.cpp "../googlepluginauthentication.cpp;../googleauthenticationinterface.cpp;../googlejob.cpp")
add_ruqola_plugin_unittest(googleauthenticationinterfacetest.cpp "../googleauthenticationinterface.cpp;../googlejob.cpp")
