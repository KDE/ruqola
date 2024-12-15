/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingslogswidget.h"
#include "apps/applicationssettingslogsinfo.h"

#include "apps/appinfojob.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <QShowEvent>
#include <QTextBrowser>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;

using namespace Qt::Literals::StringLiterals;
ApplicationsSettingsLogsWidget::ApplicationsSettingsLogsWidget(RocketChatAccount *account, QWidget *parent)
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

ApplicationsSettingsLogsWidget::~ApplicationsSettingsLogsWidget() = default;

void ApplicationsSettingsLogsWidget::showEvent(QShowEvent *event)
{
    if (!event->spontaneous() && !mWasInitialized) {
        mWasInitialized = true;
        initialize();
    }
    QWidget::showEvent(event);
}

void ApplicationsSettingsLogsWidget::generateInfo(const QJsonObject &obj)
{
    // qDebug() << " obj " << obj;
    const QJsonArray array = obj[QStringLiteral("logs")].toArray();
    QList<ApplicationsSettingsLogsInfo> logs;
    for (const auto &log : array) {
        ApplicationsSettingsLogsInfo logInfo;
        logInfo.parseLogs(log.toObject());
        logs.append(logInfo);
    }
    qDebug() << " logs " << logs;
}

void ApplicationsSettingsLogsWidget::initialize()
{
    if (mRocketChatAccount) {
        auto job = new RocketChatRestApi::AppInfoJob(this);
        job->setAppsId(mAppId);
        job->setAppInfoType(RocketChatRestApi::AppInfoJob::AppInfoType::Logs);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::AppInfoJob::appInfoDone, this, &ApplicationsSettingsLogsWidget::generateInfo);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start AppInfoJob";
        }
    }
}

void ApplicationsSettingsLogsWidget::setApplicationId(const QByteArray &appId)
{
    mAppId = appId;
}

#include "moc_applicationssettingslogswidget.cpp"
