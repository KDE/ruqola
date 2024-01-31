# SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_executable(notificationhistorygui)
target_sources(notificationhistorygui PRIVATE notificationhistorygui.h notificationhistorygui.cpp)

target_link_libraries(notificationhistorygui
    Qt::Widgets
    KF6::ItemViews
    libruqolawidgets
)
set_target_properties(unicodeemoticongui PROPERTIES DISABLE_PRECOMPILE_HEADERS ON)

