# SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_executable(loadroomcachetest)
target_sources(loadroomcachetest PRIVATE loadroomcache.cpp loadroomcache.h)
target_link_libraries(loadroomcachetest
    Qt::Widgets
    KF${KF_MAJOR_VERSION}::KIOWidgets
)
