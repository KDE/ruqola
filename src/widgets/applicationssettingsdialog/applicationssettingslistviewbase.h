/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include "misc/messagelistviewbase.h"
#include "model/appsmarketplacefilterproxymodel.h"

class ApplicationsSettingsDelegate;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsListViewBase : public MessageListViewBase
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsListViewBase(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ApplicationsSettingsListViewBase() override;

    void setFilterInfo(const AppsMarketPlaceFilterProxyModel::FilterInfo &info);
    void setSorting(AppsMarketPlaceFilterProxyModel::Sorting newSorting);

    void setRequested(bool requested);

protected:
    [[nodiscard]] bool maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    [[nodiscard]] bool mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAskApplication(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowApplicationDescription(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCustomContextMenuRequested(const QPoint &pos);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSelectAll(const QModelIndex &index);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString selectedText(const QModelIndex &index) override;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString selectedText() const;
    LIBRUQOLAWIDGETS_NO_EXPORT void slotInstallApplication(const QModelIndex &index);
    RocketChatAccount *const mRocketChatAccount;
    ApplicationsSettingsDelegate *const mApplicationsSettingsListDelegate;
    AppsMarketPlaceFilterProxyModel *const mAppsMarketPlaceFilterProxyModel;
};
