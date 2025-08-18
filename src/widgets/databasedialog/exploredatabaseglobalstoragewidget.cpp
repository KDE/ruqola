/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabaseglobalstoragewidget.h"
#include "localdatabase/localdatabasemanager.h"
#include "rocketchataccount.h"
#include "ruqola_database_widget_debug.h"
#include <QTableView>
using namespace Qt::Literals::StringLiterals;
ExploreDatabaseGlobalStorageWidget::ExploreDatabaseGlobalStorageWidget(RocketChatAccount *account, QWidget *parent)
    : ExploreDatabaseBaseStorageWidget{account, parent}
    , mLocalGlobalDatabase(account ? account->localDatabaseManager()->globalDatabase() : nullptr)
{
    if (account) {
        slotLoadModelFromDataBase(account->accountName());
    }
}

ExploreDatabaseGlobalStorageWidget::~ExploreDatabaseGlobalStorageWidget() = default;

void ExploreDatabaseGlobalStorageWidget::slotLoadModelFromDataBase(const QString &accountName)
{
    mModel = mLocalGlobalDatabase->createGlobalModel(accountName);
    if (mModel) {
        mTableView->setModel(mModel.get());
    } else {
        qCDebug(RUQOLA_DATABASE_WIDGETS_LOG) << "rooms model is nullptr" << accountName;
    }
}

#include "moc_exploredatabaseglobalstoragewidget.cpp"
