# SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_executable(expandjsontest expandjson.cpp expandjson.h)

target_link_libraries(expandjsontest
    Qt${QT_MAJOR_VERSION}::Widgets
    )

# ==================================================================== #

add_executable(unicodeemoticongui)
target_sources(unicodeemoticongui PRIVATE unicodeemoticongui.cpp unicodeemoticongui.h ../src/core/ruqolacore.qrc)

target_link_libraries(unicodeemoticongui
    Qt${QT_MAJOR_VERSION}::Widgets
    KF5::ItemViews
    libruqolacore
    )
set_target_properties(unicodeemoticongui PROPERTIES DISABLE_PRECOMPILE_HEADERS ON)
