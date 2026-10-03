# SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
ruqola_add_widgets_test_pch()

macro(add_ruqola_encryption_test _source)
    set(_test ${_source})
    get_filename_component(_name ${_source} NAME_WE)
    add_executable(
        ${_name}
        ${_test}
        ${_name}.h
    )
    add_test(NAME ${_name} COMMAND ${_name})
    if(ENABLE_PCH)
        target_precompile_headers(${_name} REUSE_FROM ${RUQOLA_TEST_PCH_TARGET})
    endif()
    ecm_mark_as_test(${_name})
    target_link_libraries(
        ${_name}
        Qt::Test
        libruqolawidgets
    )
endmacro()

add_ruqola_encryption_test(e2esaveencryptionkeywidgettest.cpp)
add_ruqola_encryption_test(e2ecopypasswordwidgettest.cpp)
add_ruqola_encryption_test(e2ecopypassworddialogtest.cpp)

add_ruqola_encryption_test(e2edecodeencryptionkeyfailedwidgettest.cpp)
add_ruqola_encryption_test(e2epassworddecodekeywidgettest.cpp)
add_ruqola_encryption_test(e2epassworddecodekeydialogtest.cpp)
add_ruqola_encryption_test(e2edecodeencryptionkeywidgettest.cpp)
add_ruqola_encryption_test(e2edisablewidgettest.cpp)
add_ruqola_encryption_test(e2edisabledialogtest.cpp)
