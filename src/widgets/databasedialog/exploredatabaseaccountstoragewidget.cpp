/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabaseaccountstoragewidget.h"

using namespace Qt::Literals::StringLiterals;
ExploreDatabaseAccountStorageWidget::ExploreDatabaseAccountStorageWidget(RocketChatAccount *account, QWidget *parent)
    : ExploreDatabaseBaseStorageWidget{account, parent}
    , mLocalAccountsDatabase(new LocalAccountsDatabase())
{
}

ExploreDatabaseAccountStorageWidget::~ExploreDatabaseAccountStorageWidget() = default;

void ExploreDatabaseAccountStorageWidget::slotLoadModelFromDataBase()
{
    // TODO
}

#include "moc_exploredatabaseaccountstoragewidget.cpp"
