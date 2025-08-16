/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabaseaccountstoragewidget.h"
#include "localdatabase/localdatabasemanager.h"
#include "rocketchataccount.h"
using namespace Qt::Literals::StringLiterals;
ExploreDatabaseAccountStorageWidget::ExploreDatabaseAccountStorageWidget(RocketChatAccount *account, QWidget *parent)
    : ExploreDatabaseBaseStorageWidget{account, parent}
    , mLocalAccountsDatabase(account ? account->localDatabaseManager()->accountDatabase() : nullptr)
{
}

ExploreDatabaseAccountStorageWidget::~ExploreDatabaseAccountStorageWidget() = default;

void ExploreDatabaseAccountStorageWidget::slotLoadModelFromDataBase(const QString &accountName)
{
    /*
    mModel = mLocalAccountsDatabase->(accountName, roomName);
    if (mModel) {
        mTableView->setModel(mModel.get());
    }
    */
    // TODO
}

#include "moc_exploredatabaseaccountstoragewidget.cpp"
