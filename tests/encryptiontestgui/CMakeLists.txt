# SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_executable(encryptiontestgui)
target_sources(
    encryptiontestgui
    PRIVATE encryptiontestgui.h encryptiontestgui.cpp
)

target_link_libraries(encryptiontestgui Qt::Widgets Qt::Gui libruqolacore)
set_target_properties(
    encryptiontestgui
    PROPERTIES DISABLE_PRECOMPILE_HEADERS ON
)
