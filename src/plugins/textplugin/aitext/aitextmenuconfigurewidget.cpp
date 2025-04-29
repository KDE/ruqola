/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "aitextmenuconfigurewidget.h"
#include "aitextmodel.h"
#include <QListView>
#include <QVBoxLayout>

AiTextMenuConfigureWidget::AiTextMenuConfigureWidget(QWidget *parent)
    : QWidget{parent}
    , mListView(new QListView(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));
    mainLayout->setContentsMargins({});

    mListView->setObjectName(QStringLiteral("mListView"));
    mainLayout->addWidget(mListView);
}

AiTextMenuConfigureWidget::~AiTextMenuConfigureWidget() = default;

#include "moc_aitextmenuconfigurewidget.cpp"
