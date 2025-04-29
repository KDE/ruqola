/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "aitextmenuconfigurewidget.h"
#include "aitextlistview.h"
#include "aitextmodel.h"
#include <QVBoxLayout>

AiTextMenuConfigureWidget::AiTextMenuConfigureWidget(QWidget *parent)
    : QWidget{parent}
    , mListView(new AiTextListView(this))
    , mModel(new AiTextModel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));
    mainLayout->setContentsMargins({});

    mListView->setObjectName(QStringLiteral("mListView"));
    mainLayout->addWidget(mListView);
    mListView->setModel(mModel);
}

AiTextMenuConfigureWidget::~AiTextMenuConfigureWidget() = default;

void AiTextMenuConfigureWidget::setAiTextInfos(const QList<AiTextInfo> &infos)
{
    mModel->setInfos(infos);
}

QList<AiTextInfo> AiTextMenuConfigureWidget::aiTextInfos() const
{
    return mModel->infos();
}

#include "moc_aitextmenuconfigurewidget.cpp"
