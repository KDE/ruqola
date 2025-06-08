# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
add_executable(
    needupdateversionwidget_gui
    needupdateversionwidget_gui.cpp
    needupdateversionmainwidget.h
    needupdateversionmainwidget.cpp
)
target_link_libraries(needupdateversionwidget_gui libruqolawidgets)
