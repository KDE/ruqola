/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "exploredatabasestoragewidget.h"
#include <QHeaderView>
#include <QSqlTableModel>
#include <QTableView>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ExploreDatabaseStorageWidget::ExploreDatabaseStorageWidget(QWidget *parent)
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

ExploreDatabaseStorageWidget::~ExploreDatabaseStorageWidget() = default;

void ExploreDatabaseStorageWidget::slotLoadModelFromDataBase(const QString &accountName, const QString &roomName)
{
    // TODO verify why we can't display it.
    auto model = mLocalMessageDatabase->createMessageModel(accountName, roomName);
    mTableView->setModel(model.get());
}

#include "moc_exploredatabasestoragewidget.cpp"
