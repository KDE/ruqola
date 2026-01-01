/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "exploredatabaseroomsubscriptionswidget.h"
#include "localdatabase/localdatabasemanager.h"
#include "rocketchataccount.h"
#include "ruqola_database_widget_debug.h"

using namespace Qt::Literals::StringLiterals;
ExploreDatabaseRoomSubscriptionsWidget::ExploreDatabaseRoomSubscriptionsWidget(RocketChatAccount *account, QWidget *parent)
    : ExploreDatabaseBaseStorageWidget{account, parent}
    , mLocalRoomSubscriptions(account ? account->localDatabaseManager()->roomSubscriptionsDatabase() : nullptr)
{
    if (account) {
        slotLoadModelFromDataBase(account->accountName());
    }
}

ExploreDatabaseRoomSubscriptionsWidget::~ExploreDatabaseRoomSubscriptionsWidget() = default;

void ExploreDatabaseRoomSubscriptionsWidget::slotLoadModelFromDataBase(const QString &accountName)
{
    mModel = mLocalRoomSubscriptions->createRoomSubscriptionsModel(accountName);
    if (mModel) {
        setModel(mModel.get());
    } else {
        qCDebug(RUQOLA_DATABASE_WIDGETS_LOG) << "rooms model is nullptr" << accountName;
    }
}
