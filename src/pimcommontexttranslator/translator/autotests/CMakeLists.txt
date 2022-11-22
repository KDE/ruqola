# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
macro(add_pimcommon_translator_unittest _source)
    get_filename_component(_name ${_source} NAME_WE)
    ecm_add_test(${_source} ${_name}.h
        TEST_NAME ${_name}
        NAME_PREFIX "pimcommon-translator-"
        LINK_LIBRARIES Qt::Test
        Qt::Widgets
        KF5::PimCommonTextTranslator
    )
endmacro()
add_pimcommon_translator_unittest(translatorwidgettest.cpp)
add_pimcommon_translator_unittest(translatorconfiguredialogtest.cpp)
add_pimcommon_translator_unittest(translatorconfigurewidgettest.cpp)
