/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferenceinfowidget.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "video-conference/videoconferenceinfojob.h"
#include <KLocalizedString>
#include <QFormLayout>
#include <QLabel>
ConferenceInfoWidget::ConferenceInfoWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mFormLayout(new QFormLayout(this))
    , mRocketChatAccount(account)
{
    mFormLayout->setContentsMargins({});
    mFormLayout->setObjectName(QStringLiteral("mFormLayout"));
}

ConferenceInfoWidget::~ConferenceInfoWidget() = default;

void ConferenceInfoWidget::initializeInfo()
{
    auto conferenceInfoJob = new RocketChatRestApi::VideoConferenceInfoJob(this);
    conferenceInfoJob->setCallId(mConferenceId);
    mRocketChatAccount->restApi()->initializeRestApiJob(conferenceInfoJob);
    connect(conferenceInfoJob, &RocketChatRestApi::VideoConferenceInfoJob::videoConferenceInfoDone, this, [this](const QJsonObject &obj) {
        qDebug() << " info " << obj;
        const QString meetingUrl = obj[QLatin1String("url")].toString();
        mFormLayout->addRow(i18n("Meeting URL:"), new QLabel(meetingUrl, this));
        const QString providerName = obj[QLatin1String("providerName")].toString();
        mFormLayout->addRow(i18n("Provider:"), new QLabel(providerName, this));

        // {"_id":"6394a19a4ef3f3baa9658f35","_updatedAt":"2022-12-10T15:11:22.376Z","anonymousUsers":0,"capabilities":{"cam":true,"mic":true,"title":true},"createdAt":"2022-12-10T15:11:22.294Z",
        // "createdBy":{"_id":"uidH","name":"Laurent Montel","username":"laurent"},
        // "messages":{"started":"QDrMfZG9BMtGQz3n6"},"providerName":"jitsi","rid":"hE6RS3iv5ND5EGWC6",
        // "ringing":true,"status":1,"success":true,"title":"ruqola225","type":"videoconference",
        // "url":"https://<url>/RocketChat6394a19a4ef3f3baa9658f35","users":[]}
    });
    if (!conferenceInfoJob->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start VideoConferenceInfoJob job";
    }
}

QString ConferenceInfoWidget::conferenceId() const
{
    return mConferenceId;
}

void ConferenceInfoWidget::setConferenceId(const QString &newConferenceId)
{
    mConferenceId = newConferenceId;
}
