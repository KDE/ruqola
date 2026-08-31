# SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
ruqola_add_widgets_test_pch()

macro(add_ruqolamisc_test _source)
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

add_ruqolamisc_test(adduserswidgettest.cpp)
add_ruqolamisc_test(clickablewidgettest.cpp)
add_ruqolamisc_test(passwordlineeditwidgettest.cpp)
add_ruqolamisc_test(passwordconfirmwidgettest.cpp)
add_ruqolamisc_test(twoauthenticationpasswordwidgettest.cpp)
add_ruqolamisc_test(systemmessagescomboboxtest.cpp)
add_ruqolamisc_test(searchwithdelaylineedittest.cpp)
add_ruqolamisc_test(accountsoverviewwidgettest.cpp)
add_ruqolamisc_test(rolescomboboxtest.cpp)
add_ruqolamisc_test(messageattachmentdownloadandsavejobtest.cpp)
add_ruqolamisc_test(soundconfigurewidgettest.cpp)
add_ruqolamisc_test(passwordvalidatewidgettest.cpp)
add_ruqolamisc_test(pixmapcachetest.cpp)
