/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsdescriptiontabwidget.h"
#include "applicationssettingsdescriptionwidget.h"
#include "applicationssettingslogswidget.h"
#include "applicationssettingssettingswidget.h"
#include "applicationssettingsversionwidget.h"
#include "rocketchataccount.h"

#include <KLocalizedString>
#include <QTabWidget>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;

ApplicationsSettingsDescriptionTabWidget::ApplicationsSettingsDescriptionTabWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mTabWidget(new QTabWidget(this))
    , mDescriptionWidget(new ApplicationsSettingsDescriptionWidget(this))
    , mVersionWidget(new ApplicationsSettingsVersionWidget(account, this))
    , mLogsWidget(new ApplicationsSettingsLogsWidget(account, this))
    , mSettingsWidget(new ApplicationsSettingsSettingsWidget(account, this))
    , mRocketChatAccount(account)
{
    mTabWidget->setObjectName(u"mTabWidget"_s);

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mainLayout->addWidget(mTabWidget);

    mDescriptionWidget->setObjectName(u"mDescriptionWidget"_s);
    mTabWidget->addTab(mDescriptionWidget, i18n("Description"));

    mVersionWidget->setObjectName(u"mVersionWidget"_s);
    mTabWidget->addTab(mVersionWidget, i18n("Versions"));

    mLogsWidget->setObjectName(u"mLogsWidget"_s);
    const int logTabIndex = mTabWidget->addTab(mLogsWidget, i18n("Logs"));

    mSettingsWidget->setObjectName(u"mSettingsWidget"_s);
    const int settingsTabIndex = mTabWidget->addTab(mSettingsWidget, i18n("Settings"));

    if (mRocketChatAccount) {
        if (!mRocketChatAccount->isAdministrator()) {
            mTabWidget->setTabVisible(logTabIndex, false);
            mTabWidget->setTabVisible(settingsTabIndex, false);
        }
    }
    mTabWidget->setTabBarAutoHide(true);
}

ApplicationsSettingsDescriptionTabWidget::~ApplicationsSettingsDescriptionTabWidget() = default;

void ApplicationsSettingsDescriptionTabWidget::setDescription(const QString &desc)
{
    mDescriptionWidget->setDescription(desc);
}

void ApplicationsSettingsDescriptionTabWidget::setShowLogAndSettingsInfo(const QByteArray &appId, bool state)
{
    if (state) {
        mLogsWidget->setApplicationId(appId);
        mSettingsWidget->setApplicationId(appId);
    }
    mTabWidget->setTabVisible(mTabWidget->indexOf(mLogsWidget), state);
    mTabWidget->setTabVisible(mTabWidget->indexOf(mSettingsWidget), state);
}

void ApplicationsSettingsDescriptionTabWidget::setShowVersionInfo(const QByteArray &appId, bool state)
{
    if (state) {
        mVersionWidget->setApplicationId(appId);
    }
    mTabWidget->setTabVisible(mTabWidget->indexOf(mVersionWidget), state);
}

#include "moc_applicationssettingsdescriptiontabwidget.cpp"
