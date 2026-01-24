# SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
macro(add_ruqolawidget_test _source)
    set(_test ${_source})
    get_filename_component(_name ${_source} NAME_WE)
    add_executable(
        ${_name}
        ${_test}
        ${_name}.h
    )
    add_test(NAME ${_name} COMMAND ${_name})
    if(ENABLE_PCH)
        target_precompile_headers(${_name} REUSE_FROM ruqolawidgetspch_tests)
    endif()
    ecm_mark_as_test(${_name})
    target_link_libraries(
        ${_name}
        Qt::Test
        libruqolawidgets
    )
    set_target_properties(
        ${_name}
        PROPERTIES
            DISABLE_PRECOMPILE_HEADERS
                ON
    )
    set_tests_properties(
        ${_name}
        PROPERTIES
            ENVIRONMENT
                "LSAN_OPTIONS=suppressions=${CMAKE_SOURCE_DIR}/sanitizers.supp"
    )
endmacro()

add_ruqolawidget_test(ruqolacentralwidgettest.cpp)

add_ruqolawidget_test(ruqolaloginwidgettest.cpp)
add_ruqolawidget_test(ruqolamainwidgettest.cpp)
add_ruqolawidget_test(ruqolamainwindowtest.cpp)
add_ruqolawidget_test(ruqolaloginbuttontest.cpp)
add_ruqolawidget_test(ruqolaswitchofflinemodewidgettest.cpp)
