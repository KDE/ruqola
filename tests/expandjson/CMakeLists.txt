# SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_executable(expandjsontest)
target_sources(expandjsontest PRIVATE expandjson.cpp expandjson.h)
target_link_libraries(expandjsontest
    Qt::Widgets
)

