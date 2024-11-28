/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include "model/appsmarketplacefilterproxymodel.h"
#include <QWidget>
class AppsCountInfoWidget;
class RocketChatAccount;
class ApplicationsSettingsListView;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsListViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsListViewWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ApplicationsSettingsListViewWidget() override;

    void setFilterInfo(const AppsMarketPlaceFilterProxyModel::FilterInfo &info);
    void setSorting(AppsMarketPlaceFilterProxyModel::Sorting newSorting);

    void setRequested(bool requested);

private:
    AppsCountInfoWidget *const mAppsCountInfoWidget;
    ApplicationsSettingsListView *const mApplicationsSettingsListView;
};
