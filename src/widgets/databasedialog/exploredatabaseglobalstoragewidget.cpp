/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabaseglobalstoragewidget.h"
#include "localdatabase/localdatabasemanager.h"
#include "rocketchataccount.h"
using namespace Qt::Literals::StringLiterals;
ExploreDatabaseGlobalStorageWidget::ExploreDatabaseGlobalStorageWidget(RocketChatAccount *account, QWidget *parent)
    : ExploreDatabaseBaseStorageWidget{account, parent}
    , mLocalGlobalDatabase(account->localDatabaseManager()->globalDatabase())
{
}

ExploreDatabaseGlobalStorageWidget::~ExploreDatabaseGlobalStorageWidget() = default;

void ExploreDatabaseGlobalStorageWidget::slotLoadModelFromDataBase()
{
    // TODO
}

#include "moc_exploredatabaseglobalstoragewidget.cpp"
