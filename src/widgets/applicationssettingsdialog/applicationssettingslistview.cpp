/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingslistview.h"
#include "applicationssettingsaskapplicationdialog.h"
#include "applicationssettingsdelegate.h"
#include "applicationssettingsdescriptiondialog.h"
#include "model/appsmarketplacefilterproxymodel.h"
#include "model/appsmarketplacemodel.h"
#include <QPointer>
using namespace Qt::Literals::StringLiterals;
ApplicationsSettingsListView::ApplicationsSettingsListView(RocketChatAccount *account, QWidget *parent)
    : QTreeView(parent)
    , mRocketChatAccount(account)
    , mApplicationsSettingsListDelegate(new ApplicationsSettingsDelegate(this))
    , mAppsMarketPlaceFilterProxyModel(new AppsMarketPlaceFilterProxyModel(this))
{
    mApplicationsSettingsListDelegate->setObjectName("mApplicationsSettingsListDelegate"_L1);
    setItemDelegate(mApplicationsSettingsListDelegate);

    mAppsMarketPlaceFilterProxyModel->setObjectName("mAppsMarketPlaceFilterProxyModel"_L1);

    setHeaderHidden(true);
    setRootIsDecorated(false);
    setItemsExpandable(false);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setIndentation(0);
    if (mRocketChatAccount) {
        mAppsMarketPlaceFilterProxyModel->setSourceModel(mRocketChatAccount->appsMarketPlaceModel());
        setModel(mAppsMarketPlaceFilterProxyModel);
    }
}

ApplicationsSettingsListView::~ApplicationsSettingsListView() = default;

void ApplicationsSettingsListView::setFilterInfo(const AppsMarketPlaceFilterProxyModel::FilterInfo &info)
{
    mAppsMarketPlaceFilterProxyModel->setFilterInfo(info);
}

void ApplicationsSettingsListView::slotAskApplication()
{
    QPointer<ApplicationsSettingsAskApplicationDialog> dlg = new ApplicationsSettingsAskApplicationDialog(this);
    if (dlg->exec()) {
        // TODO
    }
    delete dlg;
}

void ApplicationsSettingsListView::slotShowApplicationDescription()
{
    ApplicationsSettingsDescriptionDialog dlg(this);
    dlg.exec();
    // TODO
}

#include "moc_applicationssettingslistview.cpp"
