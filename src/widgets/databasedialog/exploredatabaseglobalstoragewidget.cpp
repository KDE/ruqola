/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabaseglobalstoragewidget.h"

using namespace Qt::Literals::StringLiterals;
ExploreDatabaseGlobalStorageWidget::ExploreDatabaseGlobalStorageWidget(QWidget *parent)
    : ExploreDatabaseBaseStorageWidget{parent}
    , mLocalAccountsDatabase(new LocalAccountsDatabase())
{
}

ExploreDatabaseGlobalStorageWidget::~ExploreDatabaseGlobalStorageWidget() = default;

void ExploreDatabaseGlobalStorageWidget::slotLoadModelFromDataBase()
{
    // TODO
}

#include "moc_exploredatabaseglobalstoragewidget.cpp"
