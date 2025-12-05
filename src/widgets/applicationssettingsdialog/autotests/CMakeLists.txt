# SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
macro(add_applicationssettingsdialog_test _source)
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
endmacro()

add_applicationssettingsdialog_test(applicationssettingsdialogtest.cpp)
add_applicationssettingsdialog_test(applicationssettingswidgettest.cpp)
add_applicationssettingsdialog_test(applicationssettingssearchwidgettest.cpp)
add_applicationssettingsdialog_test(applicationssettingspricecomboboxtest.cpp)
add_applicationssettingsdialog_test(applicationssettingsstatuscomboboxtest.cpp)
add_applicationssettingsdialog_test(applicationssettingsaskapplicationdialogtest.cpp)
add_applicationssettingsdialog_test(applicationssettingsaskapplicationwidgettest.cpp)
add_applicationssettingsdialog_test(applicationssettingsdescriptionwidgettest.cpp)
add_applicationssettingsdialog_test(applicationssettingssortingcomboboxtest.cpp)
add_applicationssettingsdialog_test(applicationssettingsinprogresswidgettest.cpp)
add_applicationssettingsdialog_test(appscountinfowidgettest.cpp)
add_applicationssettingsdialog_test(applicationssettingsdescriptiontabwidgettest.cpp)
add_applicationssettingsdialog_test(applicationssettingsversionwidgettest.cpp)
add_applicationssettingsdialog_test(applicationssettingslogswidgettest.cpp)
add_applicationssettingsdialog_test(applicationssettingssettingswidgettest.cpp)
add_applicationssettingsdialog_test(applicationspermissionwidgettest.cpp)
add_applicationssettingsdialog_test(applicationspermissiondialogtest.cpp)
add_applicationssettingsdialog_test(applicationssettingslistwidgettest.cpp)
