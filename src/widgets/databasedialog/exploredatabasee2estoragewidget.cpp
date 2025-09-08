/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabasee2estoragewidget.h"
#include "exploredatabaseroomiddelegate.h"
#include "exploredatabasetimestampdelegate.h"
#include "exploredatabasetooltipdelegate.h"
#include "localdatabase/localdatabasemanager.h"
#include "rocketchataccount.h"
#include "ruqola_database_widget_debug.h"
#include <QTableView>
using namespace Qt::Literals::StringLiterals;
ExploreDatabaseE2EStorageWidget::ExploreDatabaseE2EStorageWidget(RocketChatAccount *account, QWidget *parent)
    : ExploreDatabaseBaseStorageWidget{account, parent}
    , mLocalRoomsDatabase(account ? account->localDatabaseManager()->roomsDatabase() : nullptr)
{
    if (account) {
        slotLoadModelFromDataBase(account->accountName());
    }
    // RoomId => displayName
    mTableView->setItemDelegateForColumn(0, new ExploreDatabaseRoomIdDelegate(account, this));
    // timeStamp
    mTableView->setItemDelegateForColumn(1, new ExploreDatabaseTimeStampDelegate(this));

    mTableView->setItemDelegateForColumn(2, new ExploreDatabaseTooltipDelegate(this));
}

ExploreDatabaseE2EStorageWidget::~ExploreDatabaseE2EStorageWidget() = default;

void ExploreDatabaseE2EStorageWidget::slotLoadModelFromDataBase(const QString &accountName)
{
    mModel = mLocalRoomsDatabase->createRoomsModel(accountName);
    if (mModel) {
        setModel(mModel.get());
    } else {
        qCDebug(RUQOLA_DATABASE_WIDGETS_LOG) << "rooms model is nullptr" << accountName;
    }
}

#include "moc_exploredatabasee2estoragewidget.cpp"
