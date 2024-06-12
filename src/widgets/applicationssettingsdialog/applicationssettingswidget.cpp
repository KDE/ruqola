/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingswidget.h"
#include "applicationssettingslistview.h"
#include "applicationssettingssearchwidget.h"
#include "rocketchataccount.h"

#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ApplicationsSettingsWidget::ApplicationsSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mCurrentRocketChatAccount(account)
    , mApplicationsSettingsSearchWidget(new ApplicationsSettingsSearchWidget(this))
    , mApplicationsSettingsListView(new ApplicationsSettingsListView(account, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});

    mApplicationsSettingsSearchWidget->setObjectName("mApplicationsSettingsSearchWidget"_L1);
    mainLayout->addWidget(mApplicationsSettingsSearchWidget);

    mApplicationsSettingsListView->setObjectName("mApplicationsSettingsListView"_L1);
    mainLayout->addWidget(mApplicationsSettingsListView);
    if (mCurrentRocketChatAccount) {
        mCurrentRocketChatAccount->loadAppMarketPlace();
    }
}

ApplicationsSettingsWidget::~ApplicationsSettingsWidget() = default;

#include "moc_applicationssettingswidget.cpp"
