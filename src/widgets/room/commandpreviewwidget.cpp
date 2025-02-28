/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "commandpreviewwidget.h"
#include <QHBoxLayout>
#include <QListView>

CommandPreviewWidget::CommandPreviewWidget(QWidget *parent)
    : QWidget{parent}
    , mListView(new QListView(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mListView->setObjectName(QStringLiteral("mListView"));
    mainLayout->addWidget(mListView);
    mListView->setViewMode(QListView::IconMode);
    mListView->setFlow(QListView::LeftToRight);
    mListView->setResizeMode(QListView::Adjust);
    mListView->setWrapping(false);
}

CommandPreviewWidget::~CommandPreviewWidget() = default;

#include "moc_commandpreviewwidget.cpp"
