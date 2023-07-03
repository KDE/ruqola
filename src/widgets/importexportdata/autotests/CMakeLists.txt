# SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

macro(add_ruqola_importexportdata_test _source)
    set(_test ${_source})
    get_filename_component(_name ${_source} NAME_WE )
    add_executable(${_name} ${_test} ${_name}.h)
    add_test(NAME ${_name} COMMAND ${_name} )
    ecm_mark_as_test(${_name})
    target_link_libraries(${_name} Qt::Test libruqolacore libruqolawidgets)
    set_target_properties(${_name} PROPERTIES DISABLE_PRECOMPILE_HEADERS ON)
endmacro()

add_ruqola_importexportdata_test(exportdatafinishpagetest.cpp)
add_ruqola_importexportdata_test(exportdataselectaccountpagetest.cpp)
add_ruqola_importexportdata_test(exportdatawizardtest.cpp)
add_ruqola_importexportdata_test(exportaccountjobtest.cpp)

add_ruqola_importexportdata_test(importdatawizardtest.cpp)
add_ruqola_importexportdata_test(importdataselectaccountpagetest.cpp)
add_ruqola_importexportdata_test(importdatafinishpagetest.cpp)
add_ruqola_importexportdata_test(importaccountjobtest.cpp)
add_ruqola_importexportdata_test(importexportutilstest.cpp)

add_ruqola_importexportdata_test(exportdataselectaccountlistwidgettest.cpp)
