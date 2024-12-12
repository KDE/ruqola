/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsdescriptiontabwidget.h"
#include "applicationssettingsdescriptionwidget.h"
#include "applicationssettingslogswidget.h"
#include "applicationssettingsversionwidget.h"

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
{
    mTabWidget->setObjectName("mTabWidget"_L1);

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
    mainLayout->addWidget(mTabWidget);

    mDescriptionWidget->setObjectName("mDescriptionWidget"_L1);
    mTabWidget->addTab(mDescriptionWidget, i18n("Description"));

    mVersionWidget->setObjectName("mVersionWidget"_L1);
    mTabWidget->addTab(mVersionWidget, i18n("Versions"));

    mLogsWidget->setObjectName("mLogsWidget"_L1);
    mTabWidget->addTab(mLogsWidget, i18n("Logs"));
}

ApplicationsSettingsDescriptionTabWidget::~ApplicationsSettingsDescriptionTabWidget() = default;

void ApplicationsSettingsDescriptionTabWidget::setDescription(const QString &desc)
{
    mDescriptionWidget->setDescription(desc);
}

void ApplicationsSettingsDescriptionTabWidget::setApplicationId(const QByteArray &appId)
{
    mVersionWidget->setApplicationId(appId);
    mLogsWidget->setApplicationId(appId);
}

#include "moc_applicationssettingsdescriptiontabwidget.cpp"
