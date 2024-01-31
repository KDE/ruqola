# SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>
# SPDX-License-Identifier: BSD-3-Clause


add_executable(validemoticongui)
target_sources(validemoticongui PRIVATE validemoticongui.cpp validemoticongui.h)

target_link_libraries(validemoticongui
    Qt::Widgets
    KF6::ItemViews
    libruqolacore
    KF6::TextEmoticonsCore
    KF6::KIOWidgets
)
set_target_properties(validemoticongui PROPERTIES DISABLE_PRECOMPILE_HEADERS ON)
