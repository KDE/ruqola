/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "exploredatabasebasestoragewidget.h"
#include <QTableView>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ExploreDatabaseBaseStorageWidget::ExploreDatabaseBaseStorageWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mTableView(new QTableView(this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mTableView->setObjectName(u"mTableView"_s);
    mainLayout->addWidget(mTableView);
}

ExploreDatabaseBaseStorageWidget::~ExploreDatabaseBaseStorageWidget() = default;

#include "moc_exploredatabasebasestoragewidget.cpp"
