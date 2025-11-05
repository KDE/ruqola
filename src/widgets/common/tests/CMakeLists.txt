# SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_executable(
    authenticationoauthwidget_gui
    authenticationoauthwidget_gui.cpp
    authenticationoauthtestwidget.h
    authenticationoauthtestwidget.cpp
)
target_link_libraries(
    authenticationoauthwidget_gui
    libruqolawidgets
    KF6::I18n
)
