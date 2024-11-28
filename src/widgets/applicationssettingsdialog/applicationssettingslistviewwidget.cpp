/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingslistviewwidget.h"
#include "applicationssettingsdialog/applicationssettingslistview.h"
#include "appscountinfowidget.h"
#include "rocketchataccount.h"
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
ApplicationsSettingsListViewWidget::ApplicationsSettingsListViewWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mAppsCountInfoWidget(new AppsCountInfoWidget(AppsCountInfoWidget::InfoType::Unknown, this)) // TODO
    , mApplicationsSettingsListView(new ApplicationsSettingsListView(account, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});

    mAppsCountInfoWidget->setObjectName("mAppsCountInfoWidget"_L1);
    mainLayout->addWidget(mAppsCountInfoWidget);

    mApplicationsSettingsListView->setObjectName("mApplicationsSettingsListView"_L1);
    mainLayout->addWidget(mApplicationsSettingsListView);
}

ApplicationsSettingsListViewWidget::~ApplicationsSettingsListViewWidget() = default;

void ApplicationsSettingsListViewWidget::setFilterInfo(const AppsMarketPlaceFilterProxyModel::FilterInfo &info)
{
    mApplicationsSettingsListView->setFilterInfo(info);
}

void ApplicationsSettingsListViewWidget::setSorting(AppsMarketPlaceFilterProxyModel::Sorting newSorting)
{
    mApplicationsSettingsListView->setSorting(newSorting);
}

void ApplicationsSettingsListViewWidget::setRequested(bool requested)
{
    mApplicationsSettingsListView->setRequested(requested);
}

#include "moc_applicationssettingslistviewwidget.cpp"
