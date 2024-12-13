/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingslogswidget.h"

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
    const QJsonArray array = obj[QStringLiteral("apps")].toArray();
    for (const auto &info : array) {
        const QString version = info["version"_L1].toString();
        const QString changesNote = info["changesNote"_L1].toString();
        const QString createdDate = info["createdDate"_L1].toString();

        qDebug() << " version " << version << " changesNote " << changesNote << " createdDate " << createdDate;
        // TODO createdDate
    }
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
