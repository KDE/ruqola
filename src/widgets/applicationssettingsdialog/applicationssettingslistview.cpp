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
#include <QMouseEvent>
#include <QPointer>
using namespace Qt::Literals::StringLiterals;
ApplicationsSettingsListView::ApplicationsSettingsListView(RocketChatAccount *account, QWidget *parent)
    : MessageListViewBase(parent)
    , mRocketChatAccount(account)
    , mApplicationsSettingsListDelegate(new ApplicationsSettingsDelegate(account, this, this))
    , mAppsMarketPlaceFilterProxyModel(new AppsMarketPlaceFilterProxyModel(this))
{
    mApplicationsSettingsListDelegate->setObjectName("mApplicationsSettingsListDelegate"_L1);
    setItemDelegate(mApplicationsSettingsListDelegate);

    mAppsMarketPlaceFilterProxyModel->setObjectName("mAppsMarketPlaceFilterProxyModel"_L1);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    if (mRocketChatAccount) {
        mAppsMarketPlaceFilterProxyModel->setSourceModel(mRocketChatAccount->appsMarketPlaceModel());
        setModel(mAppsMarketPlaceFilterProxyModel);
    }
    connect(mApplicationsSettingsListDelegate, &ApplicationsSettingsDelegate::updateView, this, [this](const QModelIndex &index) {
        update(index);
    });
    connect(this,
            &ApplicationsSettingsListView::needToClearSizeHintCache,
            mApplicationsSettingsListDelegate,
            &ApplicationsSettingsDelegate::clearSizeHintCache);
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

bool ApplicationsSettingsListView::maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return mApplicationsSettingsListDelegate->maybeStartDrag(event, option, index);
}

bool ApplicationsSettingsListView::mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return mApplicationsSettingsListDelegate->mouseEvent(event, option, index);
}

#include "moc_applicationssettingslistview.cpp"
