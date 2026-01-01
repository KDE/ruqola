/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabaseglobalstoragewidget.h"
#include "exploredatabasetimestampdelegate.h"
#include "exploredatabasetooltipdelegate.h"
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
    mTableView->setItemDelegateForColumn(1, new ExploreDatabaseTimeStampDelegate(this));
    mTableView->setItemDelegateForColumn(0, new ExploreDatabaseTooltipDelegate(this));
}

ExploreDatabaseGlobalStorageWidget::~ExploreDatabaseGlobalStorageWidget() = default;

void ExploreDatabaseGlobalStorageWidget::slotLoadModelFromDataBase(const QString &accountName)
{
    mModel = mLocalGlobalDatabase->createGlobalModel(accountName);
    if (mModel) {
        setModel(mModel.get());
    } else {
        qCDebug(RUQOLA_DATABASE_WIDGETS_LOG) << "rooms model is nullptr" << accountName;
    }
}

#include "moc_exploredatabaseglobalstoragewidget.cpp"
