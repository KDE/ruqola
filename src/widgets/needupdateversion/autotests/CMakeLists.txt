# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
macro(add_needupdateversion_unittest _source)
    get_filename_component( _name ${_source} NAME_WE )
    ecm_add_test(${_source} ${_name}.h
        TEST_NAME ${_name}
        LINK_LIBRARIES Qt::Test Qt::Gui libruqolawidgets
    )
endmacro()

add_needupdateversion_unittest(needupdateversionwidgettest.cpp)
add_needupdateversion_unittest(needupdateversionutilstest.cpp)
