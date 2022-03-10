# SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_executable(expandjsontest expandjson.cpp expandjson.h)

target_link_libraries(expandjsontest
    Qt5::Widgets
    )

