/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "exploredatabasestoragewidget.h"
#include <QVBoxLayout>
#include <qtreeview.h>

using namespace Qt::Literals::StringLiterals;
ExploreDatabaseStorageWidget::ExploreDatabaseStorageWidget(QWidget *parent)
    : QWidget{parent}
    , mTreeView(new QTreeView(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mTreeView->setObjectName(u"mTreeView"_s);
    mainLayout->addWidget(mTreeView);
}

ExploreDatabaseStorageWidget::~ExploreDatabaseStorageWidget() = default;

#include "moc_exploredatabasestoragewidget.cpp"
