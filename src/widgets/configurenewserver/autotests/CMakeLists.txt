# SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
ruqola_add_widgets_test_pch()

macro(add_ruqolacreatenewserver_test _source)
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

add_ruqolacreatenewserver_test(createnewserverstackwidgettest.cpp)
add_ruqolacreatenewserver_test(createnewserverdialogtest.cpp)
add_ruqolacreatenewserver_test(createnewserverwidgettest.cpp)
add_ruqolacreatenewserver_test(checknewserverurlwidgettest.cpp)

if(TARGET Plasma::Activities)
    add_ruqolacreatenewserver_test(configureactivitieswidgettest.cpp)
    add_ruqolacreatenewserver_test(configureactivitiesdialogtest.cpp)
endif()
