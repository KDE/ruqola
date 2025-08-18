/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "exploredatabasemessagesstoragewidget.h"
#include "localdatabase/localdatabasemanager.h"
#include "rocketchataccount.h"
#include "ruqola_database_widget_debug.h"
#include <QHeaderView>
#include <QSqlTableModel>
#include <QTableView>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ExploreDatabaseMessagesStorageWidget::ExploreDatabaseMessagesStorageWidget(RocketChatAccount *account, QWidget *parent)
    : ExploreDatabaseBaseStorageWidget{account, parent}
    , mLocalMessageDatabase(account ? account->localDatabaseManager()->messagesDatabase() : nullptr)
{
}

ExploreDatabaseMessagesStorageWidget::~ExploreDatabaseMessagesStorageWidget() = default;

void ExploreDatabaseMessagesStorageWidget::slotLoadModelFromDataBase(const QString &accountName, const QString &roomName)
{
    mModel = mLocalMessageDatabase->createMessageModel(accountName, roomName);
    if (mModel) {
        mTableView->setModel(mModel.get());
    } else {
        qCDebug(RUQOLA_DATABASE_WIDGETS_LOG) << "rooms model is nullptr" << accountName;
    }
}

#include "moc_exploredatabasemessagesstoragewidget.cpp"
