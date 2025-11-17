/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingslistwidget.h"
#include "applicationssettingsdialog/applicationssettingsdescriptiontabwidget.h"
#include "applicationssettingsdialog/applicationssettingslistview.h"
#include "applicationssettingsdialog/applicationssettingssearchwidget.h"
#include "applicationssettingsdialog/appscountinfowidget.h"
#include "model/appsmarketplacemodel.h"
#include <QSplitter>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ApplicationsSettingsListWidget::ApplicationsSettingsListWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mApplicationsSettingsSearchWidget(new ApplicationsSettingsSearchWidget(account, this))
    , mApplicationsSettingsListView(new ApplicationsSettingsListView(account, this))
    , mAppsCountInfoWidget(new AppsCountInfoWidget(this))
    , mCurrentRocketChatAccount(account)
    , mApplicationsSettingsDescriptionTabWidget(new ApplicationsSettingsDescriptionTabWidget(account, this))
{
    mAppsCountInfoWidget->setObjectName(u"mAppsCountInfoWidget"_s);
    mAppsCountInfoWidget->setVisible(false);

    mApplicationsSettingsSearchWidget->setObjectName(u"mApplicationsSettingsSearchWidget"_s);
    mApplicationsSettingsListView->setObjectName(u"mApplicationsSettingsListView"_s);
    mApplicationsSettingsDescriptionTabWidget->hide();

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});

    auto splitter = new QSplitter(this);
    splitter->setObjectName("splitter"_L1);
    mainLayout->addWidget(splitter);

    auto appsWidget = new QWidget(this);
    appsWidget->setObjectName("appsWidget"_L1);

    auto appsWidgetLayout = new QVBoxLayout(appsWidget);
    appsWidgetLayout->setObjectName("appsWidgetLayout"_L1);
    appsWidgetLayout->setContentsMargins({});

    appsWidgetLayout->addWidget(mAppsCountInfoWidget);
    appsWidgetLayout->addWidget(mApplicationsSettingsSearchWidget);
    appsWidgetLayout->addWidget(mApplicationsSettingsListView);

    mApplicationsSettingsDescriptionTabWidget->setObjectName("mApplicationsSettingsDescriptionTabWidget"_L1);

    splitter->addWidget(appsWidget);
    splitter->addWidget(mApplicationsSettingsDescriptionTabWidget);
    splitter->setChildrenCollapsible(false);

    connect(mApplicationsSettingsSearchWidget, &ApplicationsSettingsSearchWidget::filterChanged, this, &ApplicationsSettingsListWidget::slotFilterChanged);
    connect(mApplicationsSettingsSearchWidget, &ApplicationsSettingsSearchWidget::sortingChanged, this, &ApplicationsSettingsListWidget::slotSortingChanged);
    connect(mApplicationsSettingsListView,
            &ApplicationsSettingsListView::refreshListApplications,
            this,
            &ApplicationsSettingsListWidget::slotRefreshListApplications);
    connect(mApplicationsSettingsListView,
            &ApplicationsSettingsListView::refreshCountApplications,
            this,
            &ApplicationsSettingsListWidget::slotRefreshCountApplications);

    if (mCurrentRocketChatAccount) {
        connect(mCurrentRocketChatAccount, &RocketChatAccount::appsCountLoadDone, this, [this]() {
            mAppsCountInfoWidget->setAppCountInfo(mCurrentRocketChatAccount->appsMarketPlaceModel()->appsCountInfo());
        });
    }
    connect(mApplicationsSettingsListView, &ApplicationsSettingsListView::pressed, this, &ApplicationsSettingsListWidget::slotUpdateDescription);
}

ApplicationsSettingsListWidget::~ApplicationsSettingsListWidget() = default;

void ApplicationsSettingsListWidget::slotFilterChanged()
{
    mApplicationsSettingsListView->setFilterInfo(mApplicationsSettingsSearchWidget->filterInfo());
}

void ApplicationsSettingsListWidget::slotSortingChanged()
{
    mApplicationsSettingsListView->setSorting(mApplicationsSettingsSearchWidget->sortingInfo());
}

void ApplicationsSettingsListWidget::slotRefreshCountApplications()
{
    mCurrentRocketChatAccount->updateCountApplications();
}

void ApplicationsSettingsListWidget::slotRefreshListApplications()
{
    mCurrentRocketChatAccount->updateInstalledApps();
}

void ApplicationsSettingsListWidget::setFeature(ApplicationsSettingsSearchWidget::Feature feature)
{
    mApplicationsSettingsSearchWidget->setFeature(feature);
    mAppsCountInfoWidget->setInfotype(feature == ApplicationsSettingsSearchWidget::Feature::Private ? AppsCountInfoWidget::InfoType::PrivateApps
                                                                                                    : AppsCountInfoWidget::InfoType::Applications);
    if (feature == ApplicationsSettingsSearchWidget::Feature::Requested) {
        mApplicationsSettingsListView->setRequested(true);
    }
    if (feature == ApplicationsSettingsSearchWidget::Feature::Installed) {
        mApplicationsSettingsListView->setInstalled(true);
    }
    if (feature == ApplicationsSettingsSearchWidget::Feature::Private) {
        mApplicationsSettingsListView->setIsPrivate(true);
    }
}

void ApplicationsSettingsListWidget::slotUpdateDescription(const QModelIndex &index)
{
    if (index.isValid()) {
        const QString description = index.data(AppsMarketPlaceModel::ApplicationInformations).toString();
        const QByteArray appId = index.data(AppsMarketPlaceModel::AppId).toByteArray();
        const bool showLogAndSettings = mCurrentRocketChatAccount->isAdministrator() && index.data(AppsMarketPlaceModel::Installed).toBool()
            && !index.data(AppsMarketPlaceModel::RequestedApps).toBool();
        const bool showVersion = mCurrentRocketChatAccount->isAdministrator() && !index.data(AppsMarketPlaceModel::Private).toBool();
        mApplicationsSettingsDescriptionTabWidget->setDescription(description);
        mApplicationsSettingsDescriptionTabWidget->setApplicationId(appId);
        mApplicationsSettingsDescriptionTabWidget->setShowLogAndSettingsInfo(showLogAndSettings);
        mApplicationsSettingsDescriptionTabWidget->setShowVersionInfo(showVersion);
        mApplicationsSettingsDescriptionTabWidget->show();
    } else {
        mApplicationsSettingsDescriptionTabWidget->hide();
    }
}
#include "moc_applicationssettingslistwidget.cpp"
