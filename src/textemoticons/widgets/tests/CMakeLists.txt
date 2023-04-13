# SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_executable(emoticontexteditselector_gui emoticontexteditselector_gui.cpp )

target_link_libraries(emoticontexteditselector_gui KF${KF_MAJOR_VERSION}TextEmoticonsWidgets Qt::Gui Qt::Widgets)

#####
add_executable(emoticontexteditselector_customemoji_gui emoticontexteditselector_customemoji_gui.cpp customemojiiconmanagerguitest.h customemojiiconmanagerguitest.cpp)
target_link_libraries(emoticontexteditselector_customemoji_gui KF${KF_MAJOR_VERSION}TextEmoticonsWidgets Qt::Gui Qt::Widgets)
