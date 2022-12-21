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
#include <QLabel>
#include <QVBoxLayout>
ConferenceInfoWidget::ConferenceInfoWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
}

ConferenceInfoWidget::~ConferenceInfoWidget() = default;

void ConferenceInfoWidget::initializeInfo()
{
    auto conferenceInfoJob = new RocketChatRestApi::VideoConferenceInfoJob(this);
    conferenceInfoJob->setCallId(mConferenceId);
    mRocketChatAccount->restApi()->initializeRestApiJob(conferenceInfoJob);
    connect(conferenceInfoJob, &RocketChatRestApi::VideoConferenceInfoJob::videoConferenceInfoDone, this, [this](const QJsonObject &obj) {
        qDebug() << " info " << obj;
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
