# SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
set(expandjsontest_SRCS expandjson.cpp)

add_executable(expandjsontest ${expandjsontest_SRCS})

target_link_libraries(expandjsontest
    Qt5::Widgets
    )

# ==================================================================== #

set(unicodeemoticongui_SRCS unicodeemoticongui.cpp)

qt5_add_resources(unicodeemoticongui_SRCS ../src/core/ruqolacore.qrc)

add_executable(unicodeemoticongui ${unicodeemoticongui_SRCS})

target_link_libraries(unicodeemoticongui
    Qt5::Widgets
    KF5::ItemViews
    libruqolacore
    )
set_target_properties(unicodeemoticongui PROPERTIES DISABLE_PRECOMPILE_HEADERS ON)
