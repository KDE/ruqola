# SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_executable(testshowimage_gui testshowimage_gui.cpp)
target_link_libraries(testshowimage_gui libruqolawidgets KF6::I18n)

######
add_executable(testshowvideo_gui testshowvideo_gui.cpp)
target_link_libraries(testshowvideo_gui libruqolawidgets KF6::I18n)

####
add_executable(testaudio_gui testaudio_gui.cpp)
target_link_libraries(testaudio_gui libruqolawidgets KF6::I18n)

####
add_executable(testnotification_gui)
target_sources(
    testnotification_gui
    PRIVATE testnotification_gui.cpp notificationwidget.cpp notificationwidget.h
)
target_link_libraries(testnotification_gui libruqolawidgets KF6::I18n)

####
add_executable(
    testservererrorinfo_gui
    testservererrorinfo_gui.cpp
    testservererrorinfomessagehistorywidget.h
    testservererrorinfomessagehistorywidget.cpp
)
target_link_libraries(testservererrorinfo_gui libruqolawidgets KF6::I18n)

####
add_executable(
    selectedmessagebackgroundanimation_gui
    selectedmessagebackgroundanimation_gui.cpp
    selectedmessagebackgroundanimationwidget.h
    selectedmessagebackgroundanimationwidget.cpp
)
target_link_libraries(
    selectedmessagebackgroundanimation_gui
    libruqolawidgets
    KF6::I18n
)

####
add_executable(
    testcreatevideomessagewizard_gui
    testcreatevideomessagewizard_gui.cpp
)
target_link_libraries(
    testcreatevideomessagewizard_gui
    libruqolawidgets
    KF6::I18n
)

add_executable(
    testcreatesoundmessagewizard_gui
    testcreatesoundmessagewizard_gui.cpp
)
target_link_libraries(
    testcreatesoundmessagewizard_gui
    libruqolawidgets
    KF6::I18n
)
