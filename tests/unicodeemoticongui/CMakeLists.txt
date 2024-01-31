# SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_executable(unicodeemoticongui)
target_sources(unicodeemoticongui PRIVATE unicodeemoticongui.cpp unicodeemoticongui.h ../../src/core/ruqolacore.qrc)

target_link_libraries(unicodeemoticongui
    Qt::Widgets
    KF6::ItemViews
    libruqolacore
    KF6::TextEmoticonsCore
)
set_target_properties(unicodeemoticongui PROPERTIES DISABLE_PRECOMPILE_HEADERS ON)
