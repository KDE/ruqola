# SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
macro(add_ruqola_plugin_textplugin_unittest _source _additional)
    set( _test ${_source}  ${_additional})
    get_filename_component(_name ${_source} NAME_WE)
    add_executable( ${_name} ${_test} )
    add_test(NAME ${_name} COMMAND ${_name} )

    ecm_mark_as_test(${_name})
    target_link_libraries( ${_name} Qt5::Test KF5::I18n libruqolawidgets Qt5::TextToSpeech)
endmacro ()

add_ruqola_plugin_textplugin_unittest(texttospeechtextplugintest.cpp "../texttospeechtextplugin.cpp;../texttospeechtextinterface.cpp")
add_ruqola_plugin_textplugin_unittest(texttospeechtextinterfacetest.cpp "../texttospeechtextinterface.cpp")
