/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingslistview.h"
#include "applicationssettingsdelegate.h"
#include "model/appsmarketplacefilterproxymodel.h"
#include "model/appsmarketplacemodel.h"
using namespace Qt::Literals::StringLiterals;
ApplicationsSettingsListView::ApplicationsSettingsListView(RocketChatAccount *account, QWidget *parent)
    : QTreeView(parent)
    , mApplicationsSettingsListDelegate(new ApplicationsSettingsDelegate(this))
    , mAppsMarketPlaceFilterProxyModel(new AppsMarketPlaceFilterProxyModel(this))
{
    mApplicationsSettingsListDelegate->setObjectName("mApplicationsSettingsListDelegate"_L1);
    setItemDelegate(mApplicationsSettingsListDelegate);

    mAppsMarketPlaceFilterProxyModel->setObjectName("mAppsMarketPlaceFilterProxyModel"_L1);

    setHeaderHidden(true);
    setRootIsDecorated(false);
    // setUniformRowHeights(true);
    setItemsExpandable(false);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setIndentation(0);
    if (account) {
        mAppsMarketPlaceFilterProxyModel->setSourceModel(account->appsMarketPlaceModel());
        setModel(mAppsMarketPlaceFilterProxyModel);
    }
}

ApplicationsSettingsListView::~ApplicationsSettingsListView() = default;

void ApplicationsSettingsListView::setFilterInfo(const AppsMarketPlaceFilterProxyModel::FilterInfo &info)
{
    mAppsMarketPlaceFilterProxyModel->setFilterInfo(info);
}

#include "moc_applicationssettingslistview.cpp"
