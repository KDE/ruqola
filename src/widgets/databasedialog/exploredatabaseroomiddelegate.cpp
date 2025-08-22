/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "exploredatabaseroomiddelegate.h"

ExploreDatabaseRoomIdDelegate::ExploreDatabaseRoomIdDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{
}

ExploreDatabaseRoomIdDelegate::~ExploreDatabaseRoomIdDelegate() = default;

#include "moc_exploredatabaseroomiddelegate.cpp"
