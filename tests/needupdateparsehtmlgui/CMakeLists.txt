# SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_executable(
    needupdateparsehtmlgui
    needupdateparserhtmlwidget.h
    needupdateparserhtmlwidget.cpp
)
target_sources(needupdateparsehtmlgui PRIVATE needupdateparsehtmlgui.cpp)

target_link_libraries(
    needupdateparsehtmlgui
    libruqolawidgets
    KF6::TextAddonsWidgets
)
set_target_properties(
    needupdateparsehtmlgui
    PROPERTIES
        DISABLE_PRECOMPILE_HEADERS
            ON
)
