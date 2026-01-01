/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabaseroomspendingtypedinfostoragewidget.h"
#include "exploredatabaseroomiddelegate.h"
#include "exploredatabasetooltipdelegate.h"
#include "localdatabase/localdatabasemanager.h"
#include "rocketchataccount.h"
#include "ruqola_database_widget_debug.h"
#include <QTableView>
using namespace Qt::Literals::StringLiterals;
ExploreDatabaseRoomsPendingTypedInfoStorageWidget::ExploreDatabaseRoomsPendingTypedInfoStorageWidget(RocketChatAccount *account, QWidget *parent)
    : ExploreDatabaseBaseStorageWidget{account, parent}
    , mLocalRoomsPendingTypedInfo(account ? account->localDatabaseManager()->roomPendingTypedInfoDatabase() : nullptr)
{
    if (account) {
        slotLoadModelFromDataBase(account->accountName());
    }
    // RoomId => displayName
    mTableView->setItemDelegateForColumn(0, new ExploreDatabaseRoomIdDelegate(account, this));

    mTableView->setItemDelegateForColumn(1, new ExploreDatabaseTooltipDelegate(this));
}

ExploreDatabaseRoomsPendingTypedInfoStorageWidget::~ExploreDatabaseRoomsPendingTypedInfoStorageWidget() = default;

void ExploreDatabaseRoomsPendingTypedInfoStorageWidget::slotLoadModelFromDataBase(const QString &accountName)
{
    mModel = mLocalRoomsPendingTypedInfo->createRoomsModel(accountName);
    if (mModel) {
        setModel(mModel.get());
    } else {
        qCDebug(RUQOLA_DATABASE_WIDGETS_LOG) << "rooms model is nullptr" << accountName;
    }
}

#include "moc_exploredatabaseroomspendingtypedinfostoragewidget.cpp"
