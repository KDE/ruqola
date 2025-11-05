# SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_executable(cmarktestgui)
target_sources(
    cmarktestgui
    PRIVATE
        cmarktestgui.h
        cmarktestgui.cpp
)

target_link_libraries(
    cmarktestgui
    Qt::Widgets
    Qt::Gui
    libruqolawidgets
)

if(NOT HAVE_TEXTUTILS_CMARK_SUPPORT)
    target_link_libraries(cmarktestgui cmark-rc)
endif()

set_target_properties(
    cmarktestgui
    PROPERTIES
        DISABLE_PRECOMPILE_HEADERS
            ON
)
