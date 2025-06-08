# SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_executable(passwordvalidategui)
target_sources(
    passwordvalidategui
    PRIVATE passwordvalidategui.h passwordvalidategui.cpp
)

target_link_libraries(passwordvalidategui Qt::Widgets Qt::Gui libruqolawidgets)
set_target_properties(
    passwordvalidategui
    PROPERTIES DISABLE_PRECOMPILE_HEADERS ON
)
