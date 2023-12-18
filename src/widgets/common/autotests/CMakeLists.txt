# SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
macro(add_ruqolacommon_test _source)
    set(_test ${_source})
    get_filename_component(_name ${_source} NAME_WE )
    add_executable(${_name} ${_test} ${_name}.h)
    add_test(NAME ${_name} COMMAND ${_name} )
    ecm_mark_as_test(${_name})
    target_link_libraries(${_name} Qt::Test libruqolawidgets)
    set_target_properties(${_name} PROPERTIES DISABLE_PRECOMPILE_HEADERS ON)
endmacro()

add_ruqolacommon_test(completionlistviewtest.cpp)
add_ruqolacommon_test(authenticationbuttontest.cpp)
add_ruqolacommon_test(authenticationwidgettest.cpp)
add_ruqolacommon_test(authenticationloginwidgettest.cpp)
