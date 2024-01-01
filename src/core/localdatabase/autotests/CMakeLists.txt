# SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_definitions( -DRUQOLA_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")
macro(add_ruqola_localdatabase_test _source)
    set(_test ${_source})
    get_filename_component(_name ${_source} NAME_WE )
    add_executable( ${_name} ${_test} ${ARGN} ${_name}.h)
    add_test(NAME ${_name} COMMAND ${_name} )
    ecm_mark_as_test(${_name})
    target_link_libraries( ${_name} Qt::Test libruqolacore)
    set_target_properties(${_name} PROPERTIES
        DISABLE_PRECOMPILE_HEADERS ON
    )

endmacro()
add_ruqola_localdatabase_test(localmessageloggertest.cpp)
add_ruqola_localdatabase_test(localmessagedatabasetest.cpp)
add_ruqola_localdatabase_test(localdatabaseutilstest.cpp)
add_ruqola_localdatabase_test(globaldatabasetest.cpp)

add_ruqola_localdatabase_test(localaccountdatabasetest.cpp)
add_ruqola_localdatabase_test(localroomsdatabasetest.cpp)
add_ruqola_localdatabase_test(localdatabasebasetest.cpp)
