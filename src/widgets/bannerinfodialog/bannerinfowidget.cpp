/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfowidget.h"
#include "bannerinfolistsearchlinewidget.h"
#include "bannerinfolistview.h"
#include "model/bannerinfosfilterproxymodel.h"
#include "model/bannerinfosmodel.h"
#include "rocketchataccount.h"

#include <QVBoxLayout>

BannerInfoWidget::BannerInfoWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mBannerInfoListSearchLineWidget(new BannerInfoListSearchLineWidget(this))
    , mBannerInfoListView(new BannerInfoListView(this))
    , mBannerInfosFilterProxyModel(new BannerInfosFilterProxyModel(this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mBannerInfosFilterProxyModel->setObjectName(QStringLiteral("mBannerInfosFilterProxyModel"));
    auto model = new BannerInfosModel(this);
    if (mRocketChatAccount) {
        model->insertBannerInfos(mRocketChatAccount->bannerInfos());
    }

    mBannerInfosFilterProxyModel->setSourceModel(model);
    mBannerInfoListView->setModel(mBannerInfosFilterProxyModel);

    mBannerInfoListSearchLineWidget->setObjectName(QStringLiteral("mBannerInfoListSearchLineWidget"));
    mainLayout->addWidget(mBannerInfoListSearchLineWidget);

    mBannerInfoListView->setObjectName(QStringLiteral("mBannerInfoListView"));
    mainLayout->addWidget(mBannerInfoListView);
}

BannerInfoWidget::~BannerInfoWidget() = default;
