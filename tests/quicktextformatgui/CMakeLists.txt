# SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_executable(quicktextformatgui)
target_sources(
    quicktextformatgui
    PRIVATE quicktextformatgui.cpp quicktextformatgui.h
)

target_link_libraries(quicktextformatgui libruqolawidgets)
set_target_properties(
    quicktextformatgui
    PROPERTIES DISABLE_PRECOMPILE_HEADERS ON
)
