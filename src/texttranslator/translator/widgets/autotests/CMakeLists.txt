# SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
macro(add_translatorwidget_test _source)
    set( _sources ${_source} ${ARGN} )
    get_filename_component( _name ${_source} NAME_WE )
    add_executable( ${_name} ${_sources} ${_name}.h)
    add_test(NAME ${_name} COMMAND ${_name} )
    ecm_mark_as_test(${_name})
    target_link_libraries( ${_name} Qt::Test KF5::TextTranslator)
    set_target_properties(${_name} PROPERTIES DISABLE_PRECOMPILE_HEADERS ON)
endmacro()

add_translatorwidget_test(translatorconfigurelistswidgettest.cpp)
add_translatorwidget_test(translatorconfigurelanguagelistwidgettest.cpp)
add_translatorwidget_test(translatormenutest.cpp)
