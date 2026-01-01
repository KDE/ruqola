# SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_executable(newmessageindicatorgui)
target_sources(
    newmessageindicatorgui
    PRIVATE
        newmessageindicatorgui.cpp
        newmessageindicatorgui.h
)

target_link_libraries(newmessageindicatorgui libruqolawidgets)
set_target_properties(
    newmessageindicatorgui
    PROPERTIES
        DISABLE_PRECOMPILE_HEADERS
            ON
)
