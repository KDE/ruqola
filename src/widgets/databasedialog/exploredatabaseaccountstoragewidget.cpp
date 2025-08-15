/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabaseaccountstoragewidget.h"
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ExploreDatabaseAccountStorageWidget::ExploreDatabaseAccountStorageWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
}

ExploreDatabaseAccountStorageWidget::~ExploreDatabaseAccountStorageWidget() = default;

void ExploreDatabaseAccountStorageWidget::slotLoadModelFromDataBase()
{
    // TODO
}

#include "moc_exploredatabaseaccountstoragewidget.cpp"
