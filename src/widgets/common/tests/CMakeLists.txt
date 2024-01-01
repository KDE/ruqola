# SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>
# SPDX-License-Identifier: BSD-3-Clause

add_executable(authenticationoauthwidget_gui authenticationoauthwidget_gui.cpp
    authenticationoauthtestwidget.h authenticationoauthtestwidget.cpp)
target_link_libraries(authenticationoauthwidget_gui libruqolawidgets KF${KF_MAJOR_VERSION}::I18n)
