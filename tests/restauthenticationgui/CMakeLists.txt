# SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_executable(restauthenticationgui)
target_sources(
    restauthenticationgui
    PRIVATE
        restauthenticationgui.cpp
        restauthenticationgui.h
)

target_link_libraries(
    restauthenticationgui
    Qt::Widgets
    libruqolawidgets
)
set_target_properties(
    restauthenticationgui
    PROPERTIES
        DISABLE_PRECOMPILE_HEADERS
            ON
)
