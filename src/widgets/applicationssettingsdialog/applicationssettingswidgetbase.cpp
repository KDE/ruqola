/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingswidgetbase.h"
#include "applicationssettingsinprogresswidget.h"
#include "applicationssettingslistviewbase.h"
#include "appscountinfowidget.h"
#include "memorymanager/memorymanager.h"
#include "model/appsmarketplacemodel.h"
#include "rocketchataccount.h"

#include <QStackedWidget>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ApplicationsSettingsWidgetBase::ApplicationsSettingsWidgetBase(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mCurrentRocketChatAccount(account)
    , mApplicationsSettingsSearchWidget(new ApplicationsSettingsSearchWidget(account, this))
    , mApplicationsSettingsListView(new ApplicationsSettingsListViewBase(account, this))
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
    mAppsCountInfoWidget->setVisible(false);

    mApplicationsSettingsSearchWidget->setObjectName("mApplicationsSettingsSearchWidget"_L1);
    widgetLayout->addWidget(mApplicationsSettingsSearchWidget);

    mApplicationsSettingsListView->setObjectName("mApplicationsSettingsListView"_L1);
    widgetLayout->addWidget(mApplicationsSettingsListView);

    connect(mApplicationsSettingsSearchWidget, &ApplicationsSettingsSearchWidget::filterChanged, this, &ApplicationsSettingsWidgetBase::slotFilterChanged);
    connect(mApplicationsSettingsSearchWidget, &ApplicationsSettingsSearchWidget::sortingChanged, this, &ApplicationsSettingsWidgetBase::slotSortingChanged);
}

void ApplicationsSettingsWidgetBase::initialize()
{
    if (mCurrentRocketChatAccount) {
        mCurrentRocketChatAccount->memoryManager()->stopClearApplicationSettingsModelTimer();
        mCurrentRocketChatAccount->loadAppMarketPlace();
        mCurrentRocketChatAccount->loadAppCategories();
        mCurrentRocketChatAccount->loadAppCount();
        if (mCurrentRocketChatAccount->appMarketPlaceLoaded()) {
            mStackedWidget->setCurrentWidget(mWidgetListView);
        } else {
            mStackedWidget->setCurrentWidget(mApplicationsSettingsInProgressWidget);
        }
        connect(mCurrentRocketChatAccount, &RocketChatAccount::appsMarkPlaceLoadDone, this, [this]() {
            mCurrentRocketChatAccount->loadInstalledApps();
            mStackedWidget->setCurrentWidget(mWidgetListView);
        });
        connect(mCurrentRocketChatAccount, &RocketChatAccount::appsCountLoadDone, this, [this]() {
            mAppsCountInfoWidget->setAppCountInfo(mCurrentRocketChatAccount->appsMarketPlaceModel()->appsCountInfo());
        });

    } else {
        mStackedWidget->setCurrentWidget(mWidgetListView);
    }
}

void ApplicationsSettingsWidgetBase::setFeature(ApplicationsSettingsSearchWidget::Feature feature)
{
    mApplicationsSettingsSearchWidget->setFeature(feature);
    mAppsCountInfoWidget->setInfotype(feature == ApplicationsSettingsSearchWidget::Feature::Private ? AppsCountInfoWidget::InfoType::PrivateApps
                                                                                                    : AppsCountInfoWidget::InfoType::Applications);
    if (feature == ApplicationsSettingsSearchWidget::Feature::Requested) {
        mApplicationsSettingsListView->setRequested(true);
    }
}

ApplicationsSettingsWidgetBase::~ApplicationsSettingsWidgetBase()
{
    if (mCurrentRocketChatAccount) {
        mCurrentRocketChatAccount->memoryManager()->startClearApplicationSettingsModelTimer();
    }
}

void ApplicationsSettingsWidgetBase::slotFilterChanged()
{
    mApplicationsSettingsListView->setFilterInfo(mApplicationsSettingsSearchWidget->filterInfo());
}

void ApplicationsSettingsWidgetBase::slotSortingChanged()
{
    mApplicationsSettingsListView->setSorting(mApplicationsSettingsSearchWidget->sortingInfo());
}

#include "moc_applicationssettingswidgetbase.cpp"
