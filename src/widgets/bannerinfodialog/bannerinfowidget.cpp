/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfowidget.h"
#include "bannerinfolistview.h"

#include <QVBoxLayout>

BannerInfoWidget::BannerInfoWidget(QWidget *parent)
    : QWidget{parent}
    , mBannerInfoListView(new BannerInfoListView(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mBannerInfoListView->setObjectName(QStringLiteral("mBannerInfoListView"));
    mainLayout->addWidget(mBannerInfoListView);
}

BannerInfoWidget::~BannerInfoWidget() = default;
