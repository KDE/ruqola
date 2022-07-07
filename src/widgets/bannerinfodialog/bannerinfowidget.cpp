/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfowidget.h"
#include "bannerinfolistsearchlinewidget.h"
#include "bannerinfolistview.h"
#include "model/bannerinfosfilterproxymodel.h"

#include <QVBoxLayout>

BannerInfoWidget::BannerInfoWidget(QWidget *parent)
    : QWidget{parent}
    , mBannerInfoListSearchLineWidget(new BannerInfoListSearchLineWidget(this))
    , mBannerInfoListView(new BannerInfoListView(this))
    , mBannerInfosFilterProxyModel(new BannerInfosFilterProxyModel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mBannerInfosFilterProxyModel->setObjectName(QStringLiteral("mBannerInfosFilterProxyModel"));
    // TODO mBannerInfosFilterProxyModel->setSourceModel(model);
    mBannerInfoListView->setModel(mBannerInfosFilterProxyModel);

    mBannerInfoListSearchLineWidget->setObjectName(QStringLiteral("mBannerInfoListSearchLineWidget"));
    mainLayout->addWidget(mBannerInfoListSearchLineWidget);

    mBannerInfoListView->setObjectName(QStringLiteral("mBannerInfoListView"));
    mainLayout->addWidget(mBannerInfoListView);
}

BannerInfoWidget::~BannerInfoWidget() = default;
