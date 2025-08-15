/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabaseaccountstoragewidget.h"
#include <QTableView>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ExploreDatabaseAccountStorageWidget::ExploreDatabaseAccountStorageWidget(QWidget *parent)
    : QWidget{parent}
    , mTableView(new QTableView(this))
    , mLocalAccountsDatabase(new LocalAccountsDatabase())
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mTableView->setObjectName(u"mTableView"_s);
    mainLayout->addWidget(mTableView);
}

ExploreDatabaseAccountStorageWidget::~ExploreDatabaseAccountStorageWidget() = default;

void ExploreDatabaseAccountStorageWidget::slotLoadModelFromDataBase()
{
    // TODO
}

#include "moc_exploredatabaseaccountstoragewidget.cpp"
