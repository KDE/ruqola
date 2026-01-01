/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabasee2estoragewidget.h"
#include "localdatabase/localdatabasemanager.h"
#include "rocketchataccount.h"
#include "ruqola_database_widget_debug.h"
using namespace Qt::Literals::StringLiterals;
ExploreDatabaseE2EStorageWidget::ExploreDatabaseE2EStorageWidget(RocketChatAccount *account, QWidget *parent)
    : ExploreDatabaseBaseStorageWidget{account, parent}
    , mLocalE2EDatabase(account ? account->localDatabaseManager()->e2EDatabase() : nullptr)
{
    if (account) {
        slotLoadModelFromDataBase(account->accountName());
    }
}

ExploreDatabaseE2EStorageWidget::~ExploreDatabaseE2EStorageWidget() = default;

void ExploreDatabaseE2EStorageWidget::slotLoadModelFromDataBase(const QString &accountName)
{
    /*
    mModel = mLocalRoomsDatabase->createRoomsModel(accountName);
    if (mModel) {
        setModel(mModel.get());
    } else {
        qCDebug(RUQOLA_DATABASE_WIDGETS_LOG) << "rooms model is nullptr" << accountName;
    }
    */
}

#include "moc_exploredatabasee2estoragewidget.cpp"
