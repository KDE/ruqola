/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabasee2eroomsstoragewidget.h"
#include "databasedialog/exploredatabasetooltipdelegate.h"
#include "localdatabase/localdatabasemanager.h"
#include "rocketchataccount.h"
#include "ruqola_database_widget_debug.h"
#include <QTableView>
using namespace Qt::Literals::StringLiterals;
ExploreDatabaseE2ERoomsStorageWidget::ExploreDatabaseE2ERoomsStorageWidget(RocketChatAccount *account, QWidget *parent)
    : ExploreDatabaseBaseStorageWidget{account, parent}
    , mLocalE2ERoomsDatabase(account ? account->localDatabaseManager()->e2ERoomsDataBase() : nullptr)
{
    if (account) {
        slotLoadModelFromDataBase(account->accountName());
    }
    mTableView->setItemDelegateForColumn(1, new ExploreDatabaseTooltipDelegate(this));
}

ExploreDatabaseE2ERoomsStorageWidget::~ExploreDatabaseE2ERoomsStorageWidget() = default;

void ExploreDatabaseE2ERoomsStorageWidget::slotLoadModelFromDataBase(const QString &accountName)
{
    mModel = mLocalE2ERoomsDatabase->createE2eRoomsModel(accountName);
    if (mModel) {
        setModel(mModel.get());
    } else {
        qCDebug(RUQOLA_DATABASE_WIDGETS_LOG) << "e2e rooms model is nullptr" << accountName;
    }
}

#include "moc_exploredatabasee2eroomsstoragewidget.cpp"
