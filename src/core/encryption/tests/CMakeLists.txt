# SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_executable(
    testderivekey_gui
    testderivekey_gui.cpp
    testderivekeywidget.h
    testderivekeywidget.cpp
)
target_link_libraries(
    testderivekey_gui
    libruqolacore
    Qt::Widgets
)
