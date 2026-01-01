/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

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
    const QJsonArray array = obj[u"logs"_s].toArray();
    for (const auto &log : array) {
        // qDebug() << " log " << log;
        ApplicationsSettingsLogsInfo logInfo;
        logInfo.parseLogs(log.toObject());
        if (!message.isEmpty()) {
            message += u"<br/>"_s;
        }

        message += u"<b>%1: %2 (%3 ms)</b>"_s.arg(QLocale().toString(logInfo.createdAt(), QLocale::ShortFormat),
                                                  logInfo.method(),
                                                  QString::number(logInfo.totalTime()));
        const auto arguments = logInfo.arguments();
        for (const auto &arg : arguments) {
            message += u"<br/>"_s;
            message += i18n("%1 Caller: %2", arg.severity, arg.caller) + u"<br/>"_s;
            message += arg.method + u"<br/>"_s;
            message += QString::fromLatin1(arg.args);
        }
    }
    if (message.isEmpty()) {
        message = u"<b><i>%1</i></b>"_s.arg(i18n("No logs info found."));
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
