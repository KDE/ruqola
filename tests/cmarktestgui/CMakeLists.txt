# SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_executable(cmarktestgui)
target_sources(cmarktestgui PRIVATE cmarktestgui.h cmarktestgui.cpp)

target_link_libraries(
    cmarktestgui
    Qt::Widgets
    Qt::Gui
    cmark-rc
    libruqolawidgets
)
set_target_properties(cmarktestgui PROPERTIES DISABLE_PRECOMPILE_HEADERS ON)
