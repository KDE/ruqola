# SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>
# SPDX-License-Identifier: BSD-3-Clause
add_executable(loaddatabasegui)
target_sources(loaddatabasegui PRIVATE loaddatabasegui.h loaddatabasegui.cpp)

target_link_libraries(loaddatabasegui
    Qt::Widgets
    KF6::ItemViews
    libruqolawidgets
)
set_target_properties(loaddatabasegui PROPERTIES DISABLE_PRECOMPILE_HEADERS ON)
