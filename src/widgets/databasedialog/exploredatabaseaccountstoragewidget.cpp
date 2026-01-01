/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabaseaccountstoragewidget.h"
#include "exploredatabasetooltipdelegate.h"
#include "localdatabase/localdatabasemanager.h"
#include "rocketchataccount.h"
#include "ruqola_database_widget_debug.h"
#include <QTableView>
using namespace Qt::Literals::StringLiterals;
ExploreDatabaseAccountStorageWidget::ExploreDatabaseAccountStorageWidget(RocketChatAccount *account, QWidget *parent)
    : ExploreDatabaseBaseStorageWidget{account, parent}
    , mLocalAccountsDatabase(account ? account->localDatabaseManager()->accountDatabase() : nullptr)
{
    if (account) {
        slotLoadModelFromDataBase(account->accountName());
    }
    mTableView->setItemDelegateForColumn(1, new ExploreDatabaseTooltipDelegate(this));
}

ExploreDatabaseAccountStorageWidget::~ExploreDatabaseAccountStorageWidget() = default;

void ExploreDatabaseAccountStorageWidget::slotLoadModelFromDataBase(const QString &accountName)
{
    mModel = mLocalAccountsDatabase->createAccountsModel(accountName);
    if (mModel) {
        setModel(mModel.get());
    } else {
        qCWarning(RUQOLA_DATABASE_WIDGETS_LOG) << "Model is empty for accounts";
    }
}

#include "moc_exploredatabaseaccountstoragewidget.cpp"
