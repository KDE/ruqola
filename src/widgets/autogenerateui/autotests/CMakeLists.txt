# SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_definitions(-DRUQOLA_AUTOGENERATEUI_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")
add_definitions(-DRUQOLA_BINARY_AUTOGENERATEUI_DATA_DIR="${CMAKE_CURRENT_BINARY_DIR}/data")

macro(add_ruqola_autogenerateui_test _source)
    set(_test ${_source})
    get_filename_component(_name ${_source} NAME_WE)
    add_executable(
        ${_name}
        ${_test}
        ${_name}.h
    )
    add_test(NAME ${_name} COMMAND ${_name})
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

add_ruqola_autogenerateui_test(autogenerateinteractionuitest.cpp)
add_ruqola_autogenerateui_test(autogenerateinteractionuiviewtest.cpp)
add_ruqola_autogenerateui_test(autogenerateinteractionuiviewactionabletest.cpp)
add_ruqola_autogenerateui_test(autogenerateinteractionuiviewblockstest.cpp)
add_ruqola_autogenerateui_test(autogenerateinteractionuiviewbuttonelementtest.cpp)
add_ruqola_autogenerateui_test(autogenerateinteractionuiviewtexttest.cpp)
add_ruqola_autogenerateui_test(autogenerateinteractionuiviewimagetest.cpp)
add_ruqola_autogenerateui_test(autogenerateinteractionuiviewplaintextinputelementtest.cpp)
add_ruqola_autogenerateui_test(autogenerateinteractionuiviewimageelementtest.cpp)
add_ruqola_autogenerateui_test(autogenerateinteractionuiviewoverflowelementtest.cpp)
add_ruqola_autogenerateui_test(autogenerateinteractionuiviewinputblocktest.cpp)
add_ruqola_autogenerateui_test(autogenerateinteractionuiviewdatepickerelementtest.cpp)
