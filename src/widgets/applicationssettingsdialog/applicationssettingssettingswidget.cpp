/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingssettingswidget.h"

#include "apps/appinfojob.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <QShowEvent>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;

using namespace Qt::Literals::StringLiterals;
ApplicationsSettingsSettingsWidget::ApplicationsSettingsSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mRocketChatAccount(account)
    , mMainLayout(new QVBoxLayout(this))
{
    mMainLayout->setObjectName("mainLayout"_L1);
    mMainLayout->setContentsMargins({});
}

ApplicationsSettingsSettingsWidget::~ApplicationsSettingsSettingsWidget() = default;

void ApplicationsSettingsSettingsWidget::showEvent(QShowEvent *event)
{
    if (!event->spontaneous() && !mWasInitialized) {
        mWasInitialized = true;
        initialize();
    }
    QWidget::showEvent(event);
}

void ApplicationsSettingsSettingsWidget::generateSettings(const QJsonObject &obj)
{
    // TODO use mMainLayout
    qDebug() << " obj " << obj;
    const QJsonArray array = obj[QStringLiteral("apps")].toArray();
    for (const auto &info : array) {
        const QString version = info["version"_L1].toString();
        const QString changesNote = info["changesNote"_L1].toString();
        const QString createdDate = info["createdDate"_L1].toString();

        qDebug() << " version " << version << " changesNote " << changesNote << " createdDate " << createdDate;
        // TODO createdDate
    }
}

void ApplicationsSettingsSettingsWidget::initialize()
{
    if (mRocketChatAccount) {
        auto job = new RocketChatRestApi::AppInfoJob(this);
        job->setAppsId(mAppId);
        job->setAppInfoType(RocketChatRestApi::AppInfoJob::AppInfoType::Settings);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::AppInfoJob::appInfoDone, this, &ApplicationsSettingsSettingsWidget::generateSettings);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start AppInfoJob";
        }
    }
}

void ApplicationsSettingsSettingsWidget::setApplicationId(const QByteArray &appId)
{
    mAppId = appId;
}

#include "moc_applicationssettingssettingswidget.cpp"
