# SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_executable(expandjsontest expandjson.cpp)

target_link_libraries(expandjsontest
    Qt5::Widgets
    )

# ==================================================================== #

add_executable(unicodeemoticongui)
target_sources(unicodeemoticongui PRIVATE unicodeemoticongui.cpp ../src/core/ruqolacore.qrc)

target_link_libraries(unicodeemoticongui
    Qt5::Widgets
    KF5::ItemViews
    libruqolacore
    )
set_target_properties(unicodeemoticongui PROPERTIES DISABLE_PRECOMPILE_HEADERS ON)
