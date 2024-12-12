/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsversionwidget.h"
#include "apps/appinfojob.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <QShowEvent>
#include <QTextBrowser>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ApplicationsSettingsVersionWidget::ApplicationsSettingsVersionWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mTextBrowser(new QTextBrowser(this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});

    mTextBrowser->setObjectName("mTextBrowser"_L1);
    mainLayout->addWidget(mTextBrowser);
    mTextBrowser->setOpenExternalLinks(true);
}

void ApplicationsSettingsVersionWidget::setApplicationId(const QByteArray &appId)
{
    mAppId = appId;
}

ApplicationsSettingsVersionWidget::~ApplicationsSettingsVersionWidget() = default;

void ApplicationsSettingsVersionWidget::showEvent(QShowEvent *event)
{
    if (!event->spontaneous() && !mWasInitialized) {
        mWasInitialized = true;
        initialize();
    }
    QWidget::showEvent(event);
}

void ApplicationsSettingsVersionWidget::initialize()
{
    if (mRocketChatAccount) {
        auto job = new RocketChatRestApi::AppInfoJob(this);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        job->setAppInfoType(RocketChatRestApi::AppInfoJob::AppInfoType::Versions);
        job->setAppsId(mAppId);
        connect(job, &RocketChatRestApi::AppInfoJob::appInfoDone, this, &ApplicationsSettingsVersionWidget::generateInfo);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start AppInfoJob";
        }
    }
}

void ApplicationsSettingsVersionWidget::generateInfo(const QJsonObject &obj)
{
    qDebug() << " obj " << obj;
    // TODO
}

#include "moc_applicationssettingsversionwidget.cpp"
