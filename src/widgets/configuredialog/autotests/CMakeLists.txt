# SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
macro(add_ruqolaconfiguredialog_test _source)
    set( _test ${_source})
    get_filename_component( _name ${_source} NAME_WE )
    add_executable( ${_name} ${_test} ${_name}.h)
    add_test(NAME ${_name} COMMAND ${_name} )
    ecm_mark_as_test(${_name})
    target_link_libraries( ${_name} Qt::Test libruqolacore libruqolawidgets)
    set_target_properties(${_name} PROPERTIES DISABLE_PRECOMPILE_HEADERS ON)
endmacro()

add_ruqolaconfiguredialog_test(configuresettingsdialogtest.cpp)
add_ruqolaconfiguredialog_test(configuregeneralwidgettest.cpp)
add_ruqolaconfiguredialog_test(configurefontwidgettest.cpp)

if (HAVE_TEXT_AUTOCORRECTION)
    add_ruqolaconfiguredialog_test(configureautocorrectionwidgettest.cpp)
endif()

if (HAVE_TEXT_TO_SPEECH)
    add_ruqolaconfiguredialog_test(configureaccessibilitywidgettest.cpp)
endif()
