# SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_executable(loaddatabasegui)
target_sources(loaddatabasegui PRIVATE loaddatabasegui.h loaddatabasegui.cpp)

target_link_libraries(loaddatabasegui
    Qt::Widgets
    KF${KF_MAJOR_VERSION}::ItemViews
    libruqolawidgets
)
set_target_properties(loaddatabasegui PROPERTIES DISABLE_PRECOMPILE_HEADERS ON)
