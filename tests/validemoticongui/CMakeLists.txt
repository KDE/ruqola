# SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>
# SPDX-License-Identifier: BSD-3-Clause


add_executable(validemoticongui)
target_sources(validemoticongui PRIVATE validemoticongui.cpp validemoticongui.h)

target_link_libraries(validemoticongui
    Qt::Widgets
    KF${KF_MAJOR_VERSION}::ItemViews
    libruqolacore
    KF${KF_MAJOR_VERSION}::TextEmoticonsCore
    KF${KF_MAJOR_VERSION}::KIOWidgets
)
set_target_properties(validemoticongui PROPERTIES DISABLE_PRECOMPILE_HEADERS ON)
