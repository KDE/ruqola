# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none

add_definitions(-DRUQOLA_DATA_NEEDUPDATEDIR="${CMAKE_CURRENT_SOURCE_DIR}/data")

macro(add_needupdateversion_unittest _source)
    get_filename_component(_name ${_source} NAME_WE)
    ecm_add_test(${_source} ${_name}.h
        TEST_NAME ${_name}
        LINK_LIBRARIES Qt::Test Qt::Gui libruqolawidgets
    )
endmacro()

add_needupdateversion_unittest(needupdateversionwidgettest.cpp)
add_needupdateversion_unittest(needupdateversionutilstest.cpp)
add_needupdateversion_unittest(needupdateparsehtmljobtest.cpp)
add_needupdateversion_unittest(needupdateparsehtmlutiltest.cpp)
add_needupdateversion_unittest(needupdatecheckexistingnewversionjobtest.cpp)
