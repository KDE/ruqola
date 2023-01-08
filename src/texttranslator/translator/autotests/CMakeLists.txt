# SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
macro(add_translator_unittest _source)
    get_filename_component(_name ${_source} NAME_WE)
    ecm_add_test(${_source} ${_name}.h
        TEST_NAME ${_name}
        NAME_PREFIX "translator-"
        LINK_LIBRARIES Qt::Test
        Qt::Widgets
        KF5::TextTranslator
    )
endmacro()
add_translator_unittest(translatorwidgettest.cpp)
add_translator_unittest(translatorconfiguredialogtest.cpp)
add_translator_unittest(translatorconfigurewidgettest.cpp)
add_translator_unittest(translatorengineloadertest.cpp)
add_translator_unittest(translatorconfigurecombowidgettest.cpp)
