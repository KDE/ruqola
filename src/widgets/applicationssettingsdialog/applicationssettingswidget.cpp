/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingswidget.h"
#include "applicationssettingsinprogresswidget.h"
#include "applicationssettingslistview.h"
#include "appscountinfowidget.h"
#include "memorymanager/memorymanager.h"
#include "model/appsmarketplacemodel.h"
#include "rocketchataccount.h"

#include <QStackedWidget>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ApplicationsSettingsWidget::ApplicationsSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mCurrentRocketChatAccount(account)
    , mApplicationsSettingsSearchWidget(new ApplicationsSettingsSearchWidget(account, this))
    , mApplicationsSettingsListView(new ApplicationsSettingsListView(account, this))
    , mApplicationsSettingsInProgressWidget(new ApplicationsSettingsInProgressWidget(this))
    , mAppsCountInfoWidget(new AppsCountInfoWidget(this))
    , mStackedWidget(new QStackedWidget(this))
    , mWidgetListView(new QWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});

    mStackedWidget->setObjectName("mStackedWidget"_L1);
    mainLayout->addWidget(mStackedWidget);

    mApplicationsSettingsInProgressWidget->setObjectName("mApplicationsSettingsInProgressWidget"_L1);
    mStackedWidget->addWidget(mApplicationsSettingsInProgressWidget);

    auto widgetLayout = new QVBoxLayout(mWidgetListView);
    widgetLayout->setObjectName("widgetLayout"_L1);
    widgetLayout->setContentsMargins({});

    mWidgetListView->setObjectName("mWidgetListView"_L1);
    mStackedWidget->addWidget(mWidgetListView);

    mAppsCountInfoWidget->setObjectName("mAppsCountInfoWidget"_L1);
    widgetLayout->addWidget(mAppsCountInfoWidget);

    mApplicationsSettingsSearchWidget->setObjectName("mApplicationsSettingsSearchWidget"_L1);
    widgetLayout->addWidget(mApplicationsSettingsSearchWidget);

    mApplicationsSettingsListView->setObjectName("mApplicationsSettingsListView"_L1);
    widgetLayout->addWidget(mApplicationsSettingsListView);

    if (mCurrentRocketChatAccount) {
        mCurrentRocketChatAccount->memoryManager()->stopClearApplicationSettingsModelTimer();
        mCurrentRocketChatAccount->loadAppMarketPlace();
        mCurrentRocketChatAccount->loadAppCategories();
        mCurrentRocketChatAccount->loadInstalledApps();
        mCurrentRocketChatAccount->loadAppCount();
        if (mCurrentRocketChatAccount->appMarketPlaceLoaded()) {
            mStackedWidget->setCurrentWidget(mWidgetListView);
        } else {
            mStackedWidget->setCurrentWidget(mApplicationsSettingsInProgressWidget);
        }
        connect(mCurrentRocketChatAccount, &RocketChatAccount::appsMarkPlaceLoadDone, this, [this]() {
            mStackedWidget->setCurrentWidget(mWidgetListView);
        });
        connect(mCurrentRocketChatAccount, &RocketChatAccount::appsCountLoadDone, this, [this]() {
            // TODO fix me
            mAppsCountInfoWidget->setAppCountInfo(mCurrentRocketChatAccount->appsMarketPlaceModel()->appsCountInfo(),
                                                  AppsCountInfoWidget::InfoType::Applications);
        });

    } else {
        mStackedWidget->setCurrentWidget(mWidgetListView);
    }
    connect(mApplicationsSettingsSearchWidget, &ApplicationsSettingsSearchWidget::filterChanged, this, &ApplicationsSettingsWidget::slotFilterChanged);
    connect(mApplicationsSettingsSearchWidget, &ApplicationsSettingsSearchWidget::sortingChanged, this, &ApplicationsSettingsWidget::slotSortingChanged);
}

void ApplicationsSettingsWidget::setFeature(ApplicationsSettingsSearchWidget::Feature feature)
{
    mApplicationsSettingsSearchWidget->setFeature(feature);
    if (feature == ApplicationsSettingsSearchWidget::Feature::Requested) {
        mApplicationsSettingsListView->setRequested(true);
    }
}

ApplicationsSettingsWidget::~ApplicationsSettingsWidget()
{
    if (mCurrentRocketChatAccount) {
        mCurrentRocketChatAccount->memoryManager()->startClearApplicationSettingsModelTimer();
    }
}

void ApplicationsSettingsWidget::slotFilterChanged()
{
    mApplicationsSettingsListView->setFilterInfo(mApplicationsSettingsSearchWidget->filterInfo());
}

void ApplicationsSettingsWidget::slotSortingChanged()
{
    mApplicationsSettingsListView->setSorting(mApplicationsSettingsSearchWidget->sortingInfo());
}

#include "moc_applicationssettingswidget.cpp"
