/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include "misc/messagelistviewbase.h"
#include "model/appsmarketplacefilterproxymodel.h"
#include "rocketchataccount.h"
#include <QTreeView>
class ApplicationsSettingsDelegate;
class RocketChatAccount;
class AppsMarketPlaceFilterProxyModel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsListView : public MessageListViewBase
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsListView(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ApplicationsSettingsListView() override;

    void setFilterInfo(const AppsMarketPlaceFilterProxyModel::FilterInfo &info);

protected:
    [[nodiscard]] bool maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    [[nodiscard]] bool mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAskApplication();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowApplicationDescription();
    RocketChatAccount *const mRocketChatAccount;
    ApplicationsSettingsDelegate *const mApplicationsSettingsListDelegate;
    AppsMarketPlaceFilterProxyModel *const mAppsMarketPlaceFilterProxyModel;
};
