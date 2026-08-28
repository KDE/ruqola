/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfowidget.h"

#include "bannerinfolistsearchlinewidget.h"
#include "bannerinfolistview.h"
#include "model/bannerinfosfilterproxymodel.h"
#include "model/bannerinfosmodel.h"
#include "rocketchataccount.h"

#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
BannerInfoWidget::BannerInfoWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mBannerInfoListSearchLineWidget(new BannerInfoListSearchLineWidget(this))
    , mBannerInfoListView(new BannerInfoListView(account, this))
    , mBannerInfosFilterProxyModel(new BannerInfosFilterProxyModel(this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(u"mainLayout"_s);

    mBannerInfosFilterProxyModel->setObjectName(u"mBannerInfosFilterProxyModel"_s);
    auto model = new BannerInfosModel(this);
    if (mRocketChatAccount) {
        model->insertBannerInfos(mRocketChatAccount->bannerInfos());
    }

    mBannerInfosFilterProxyModel->setSourceModel(model);
    mBannerInfoListView->setModel(mBannerInfosFilterProxyModel);

    mBannerInfoListSearchLineWidget->setObjectName(u"mBannerInfoListSearchLineWidget"_s);
    mainLayout->addWidget(mBannerInfoListSearchLineWidget);

    mBannerInfoListView->setObjectName(u"mBannerInfoListView"_s);
    mainLayout->addWidget(mBannerInfoListView);

    connect(mBannerInfoListSearchLineWidget, &BannerInfoListSearchLineWidget::filterChanged, this, &BannerInfoWidget::initializeProxyModel);
    initializeProxyModel();
}

BannerInfoWidget::~BannerInfoWidget() = default;

void BannerInfoWidget::initializeProxyModel()
{
    mBannerInfosFilterProxyModel->setFilterFixedString(mBannerInfoListSearchLineWidget->searchText());
    mBannerInfosFilterProxyModel->setShowUnread(mBannerInfoListSearchLineWidget->showOnlyUnread());
    mBannerInfoListView->setSearchText(mBannerInfoListSearchLineWidget->searchText());
}

#include "moc_bannerinfowidget.cpp"
