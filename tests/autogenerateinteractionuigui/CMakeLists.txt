# SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_executable(autogenerateinteractiongui)
target_sources(
    autogenerateinteractiongui
    PRIVATE autogenerateinteractiongui.cpp autogenerateinteractiongui.h
)

target_link_libraries(autogenerateinteractiongui Qt::Widgets libruqolawidgets)

set_target_properties(
    autogenerateinteractiongui
    PROPERTIES DISABLE_PRECOMPILE_HEADERS ON
)
