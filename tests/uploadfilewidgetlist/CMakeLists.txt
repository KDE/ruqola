# SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_executable(uploadfilewidgetlistgui)
target_sources(uploadfilewidgetlistgui PRIVATE main.cpp uploadwidget.h uploadwidget.cpp)

target_link_libraries(uploadfilewidgetlistgui
    Qt::Widgets
    libruqolawidgets
)
set_target_properties(uploadfilewidgetlistgui PROPERTIES DISABLE_PRECOMPILE_HEADERS ON)
