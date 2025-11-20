# SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_executable(textdocumenttestgui)
target_sources(
    textdocumenttestgui
    PRIVATE
        textdocumenttestgui.cpp
        textdocumenttestgui.h
)

target_link_libraries(textdocumenttestgui libruqolawidgets)

set_target_properties(
    textdocumenttestgui
    PROPERTIES
        DISABLE_PRECOMPILE_HEADERS
            ON
)
