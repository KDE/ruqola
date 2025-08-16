/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabaseglobalstoragewidget.h"
#include "localdatabase/localdatabasemanager.h"
#include "rocketchataccount.h"
#include <QTableView>
using namespace Qt::Literals::StringLiterals;
ExploreDatabaseGlobalStorageWidget::ExploreDatabaseGlobalStorageWidget(RocketChatAccount *account, QWidget *parent)
    : ExploreDatabaseBaseStorageWidget{account, parent}
    , mLocalGlobalDatabase(account ? account->localDatabaseManager()->globalDatabase() : nullptr)
{
}

ExploreDatabaseGlobalStorageWidget::~ExploreDatabaseGlobalStorageWidget() = default;

void ExploreDatabaseGlobalStorageWidget::slotLoadModelFromDataBase(const QString &accountName)
{
    mModel = mLocalGlobalDatabase->createGlobalModel(accountName);
    if (mModel) {
        mTableView->setModel(mModel.get());
    }

    // TODO
}

#include "moc_exploredatabaseglobalstoragewidget.cpp"
