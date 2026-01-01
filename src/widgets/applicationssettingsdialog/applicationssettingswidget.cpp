/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingswidget.h"
#include "applicationssettingsdialog/applicationssettingslistwidget.h"
#include "applicationssettingsinprogresswidget.h"
#include "applicationssettingslistview.h"
#include "appscountinfowidget.h"
#include "memorymanager/memorymanager.h"
#include "model/appsmarketplacemodel.h"

#include <QStackedWidget>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ApplicationsSettingsWidget::ApplicationsSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mCurrentRocketChatAccount(account)
    , mApplicationsSettingsListWidget(new ApplicationsSettingsListWidget(account, this))
    , mApplicationsSettingsInProgressWidget(new ApplicationsSettingsInProgressWidget(this))
    , mStackedWidget(new QStackedWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});

    mStackedWidget->setObjectName("mStackedWidget"_L1);
    mainLayout->addWidget(mStackedWidget);

    mApplicationsSettingsInProgressWidget->setObjectName("mApplicationsSettingsInProgressWidget"_L1);
    mStackedWidget->addWidget(mApplicationsSettingsInProgressWidget);

    mApplicationsSettingsListWidget->setObjectName("mApplicationsSettingsListWidget"_L1);
    mStackedWidget->addWidget(mApplicationsSettingsListWidget);
}

void ApplicationsSettingsWidget::initialize()
{
    if (mCurrentRocketChatAccount) {
        mCurrentRocketChatAccount->memoryManager()->stopClearApplicationSettingsModelTimer();
        mCurrentRocketChatAccount->loadAppMarketPlace();
        if (mCurrentRocketChatAccount->appMarketPlaceLoaded()) {
            mStackedWidget->setCurrentWidget(mApplicationsSettingsListWidget);
        } else {
            mStackedWidget->setCurrentWidget(mApplicationsSettingsInProgressWidget);
        }
        connect(mCurrentRocketChatAccount, &RocketChatAccount::appsMarkPlaceLoadDone, this, [this]() {
            mStackedWidget->setCurrentWidget(mApplicationsSettingsListWidget);
        });
    } else {
        mStackedWidget->setCurrentWidget(mApplicationsSettingsListWidget);
    }
}

void ApplicationsSettingsWidget::setFeature(ApplicationsSettingsSearchWidget::Feature feature)
{
    mApplicationsSettingsListWidget->setFeature(feature);
}

ApplicationsSettingsWidget::~ApplicationsSettingsWidget()
{
    if (mCurrentRocketChatAccount) {
        mCurrentRocketChatAccount->memoryManager()->startClearApplicationSettingsModelTimer();
    }
}

#include "moc_applicationssettingswidget.cpp"
