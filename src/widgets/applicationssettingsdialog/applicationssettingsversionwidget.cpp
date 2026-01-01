/*
SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsversionwidget.h"
#include "apps/appinfojob.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
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
        mTextBrowser->setText(u"<b><i>%1</i></b>"_s.arg(i18n("Fetching version information in progress...")));
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
    QString message;
    const QJsonArray array = obj[u"apps"_s].toArray();
    for (const auto &info : array) {
        const QString version = info["version"_L1].toString();
        QString changesNote = info["changesNote"_L1].toString();
        if (changesNote.isEmpty()) {
            changesNote = i18n("No release information provided");
        }
        // qDebug() << " version " << version << " changesNote " << changesNote << " createdDate " << createdDate;

        message += u"<b>%1</b><br/>"_s.arg(version);
        message += changesNote + u"<br/><br/>"_s;
        // const QString createdDate = info["createdDate"_L1].toString();
        // TODO add createdDate
    }
    if (message.isEmpty()) {
        message = u"<b><i>%1</i></b>"_s.arg(i18n("No version info found."));
    }
    mTextBrowser->setText(message);
}

#include "moc_applicationssettingsversionwidget.cpp"
