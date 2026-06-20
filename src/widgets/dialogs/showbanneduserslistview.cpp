/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showbanneduserslistview.h"

ShowBannedUsersListView::ShowBannedUsersListView(QWidget *parent)
    : QListView(parent)
{
}

ShowBannedUsersListView::~ShowBannedUsersListView() = default;
#include "moc_showbanneduserslistview.cpp"
