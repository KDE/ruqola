# SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

ecm_qt_declare_logging_category(ruqola_personalaccesstokenauthenticationplugin_test_SRCS HEADER personalaccesstokenauthenticationplugin_debug.h
    IDENTIFIER RUQOLA_PERSONALACCESSTOKENAUTHENTICATION_PLUGIN_LOG CATEGORY_NAME org.kde.ruqola_personalaccesstokenauthenticationplugin
)

macro(add_ruqola_personalaccesstoken_unittest _source _additional)
    set(_test
        ${ruqola_personalaccesstokenauthenticationplugin_test_SRCS}
        ${_source}
        ${_additional}
    )
    get_filename_component(_name ${_source} NAME_WE)
    add_executable(${_name} ${_test} ${_name}.h)
    add_test(NAME ${_name} COMMAND ${_name})

    ecm_mark_as_test(${_name})
    target_link_libraries(
        ${_name}
        Qt::Test
        KF6::I18n
        libruqolacore
        Qt::Widgets
        KF6::WidgetsAddons
    )
    set_target_properties(${_name} PROPERTIES DISABLE_PRECOMPILE_HEADERS ON)
endmacro()

add_ruqola_personalaccesstoken_unittest(personalaccesstokenauthenticationconfigwidgettest.cpp
    "../personalaccesstokenauthenticationconfigwidget.cpp"
)
