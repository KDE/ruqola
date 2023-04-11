# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
macro(add_textemoticon_unittest _source)
    get_filename_component(_name ${_source} NAME_WE)
    ecm_add_test(${_source} ${_name}.h TEST_NAME ${_name}
        LINK_LIBRARIES Qt::Test KF${KF_MAJOR_VERSION}::TextEmoticonsCore
        )
endmacro()

add_textemoticon_unittest( emoticoncategorytest.cpp )
