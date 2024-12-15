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
#include <KLocalizedString>
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
    QString message;
    // qDebug() << " obj " << obj;
    const QJsonArray array = obj[QStringLiteral("logs")].toArray();
    for (const auto &log : array) {
        ApplicationsSettingsLogsInfo logInfo;
        logInfo.parseLogs(log.toObject());
        if (!message.isEmpty()) {
            message += QStringLiteral("<br/>");
        }

        message += QStringLiteral("<b>") + QLocale().toString(logInfo.createdAt());
        message += logInfo.method() + QStringLiteral("</b>");
        for (const auto &arg : logInfo.arguments()) {
            message += QStringLiteral("<br/>");
            message += arg.caller + QStringLiteral("<br/>");
            message += arg.method + QStringLiteral("<br/>");
            message += arg.severity + QStringLiteral("<br/>");
            for (const QString &str : arg.args) {
                message += str + QStringLiteral("<br/>");
            }
        }
    }
    if (message.isEmpty()) {
        message = QStringLiteral("<b><i>%1</i></b>").arg(i18n("No logs info found."));
    }
    mTextBrowser->setHtml(message);
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
