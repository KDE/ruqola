# SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_executable(whatsnewdialoggui)
target_sources(whatsnewdialoggui PRIVATE whatsnewdialoggui.cpp)

target_link_libraries(whatsnewdialoggui libruqolawidgets)
set_target_properties(
    whatsnewdialoggui
    PROPERTIES DISABLE_PRECOMPILE_HEADERS ON
)
