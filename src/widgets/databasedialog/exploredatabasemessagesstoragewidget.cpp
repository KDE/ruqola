/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "exploredatabasemessagesstoragewidget.h"
#include <QHeaderView>
#include <QSqlTableModel>
#include <QTableView>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ExploreDatabaseMessagesStorageWidget::ExploreDatabaseMessagesStorageWidget(QWidget *parent)
    : QWidget{parent}
    , mTableView(new QTableView(this))
    , mLocalMessageDatabase(new LocalMessagesDatabase())
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mTableView->setObjectName(u"mTableView"_s);
    mainLayout->addWidget(mTableView);
}

ExploreDatabaseMessagesStorageWidget::~ExploreDatabaseMessagesStorageWidget() = default;

void ExploreDatabaseMessagesStorageWidget::slotLoadModelFromDataBase(const QString &accountName, const QString &roomName)
{
    mModel = mLocalMessageDatabase->createMessageModel(accountName, roomName);
    if (mModel) {
        mTableView->setModel(mModel.get());
    }
}

#include "moc_exploredatabasemessagesstoragewidget.cpp"
