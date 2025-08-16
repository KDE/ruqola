/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabaseroomsstoragewidget.h"
#include "localdatabase/localdatabasemanager.h"
#include "rocketchataccount.h"
using namespace Qt::Literals::StringLiterals;
ExploreDatabaseRoomsStorageWidget::ExploreDatabaseRoomsStorageWidget(RocketChatAccount *account, QWidget *parent)
    : ExploreDatabaseBaseStorageWidget{account, parent}
    , mLocalRoomsDatabase(account->localDatabaseManager()->roomsDatabase())
{
}

ExploreDatabaseRoomsStorageWidget::~ExploreDatabaseRoomsStorageWidget() = default;

void ExploreDatabaseRoomsStorageWidget::slotLoadModelFromDataBase()
{
    // TODO
}

#include "moc_exploredatabaseroomsstoragewidget.cpp"
