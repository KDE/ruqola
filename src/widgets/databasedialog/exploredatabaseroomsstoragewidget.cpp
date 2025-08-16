/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabaseroomsstoragewidget.h"
#include "localdatabase/localdatabasemanager.h"
#include "rocketchataccount.h"
#include <QTableView>
using namespace Qt::Literals::StringLiterals;
ExploreDatabaseRoomsStorageWidget::ExploreDatabaseRoomsStorageWidget(RocketChatAccount *account, QWidget *parent)
    : ExploreDatabaseBaseStorageWidget{account, parent}
    , mLocalRoomsDatabase(account ? account->localDatabaseManager()->roomsDatabase() : nullptr)
{
    if (account) {
        slotLoadModelFromDataBase(account->accountName());
    }
}

ExploreDatabaseRoomsStorageWidget::~ExploreDatabaseRoomsStorageWidget() = default;

void ExploreDatabaseRoomsStorageWidget::slotLoadModelFromDataBase(const QString &accountName)
{
    mModel = mLocalRoomsDatabase->createRoomsModel(accountName);
    if (mModel) {
        mTableView->setModel(mModel.get());
    }
}

#include "moc_exploredatabaseroomsstoragewidget.cpp"
