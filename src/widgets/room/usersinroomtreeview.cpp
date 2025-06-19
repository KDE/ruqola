/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersinroomtreeview.h"

UsersInRoomTreeView::UsersInRoomTreeView(QWidget *parent)
    : QTreeView(parent)
{
}

UsersInRoomTreeView::~UsersInRoomTreeView() = default;

#include "moc_usersinroomtreeview.cpp"
