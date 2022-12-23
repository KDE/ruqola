/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferencedirectcalldialog.h"
#include "connection.h"
#include "ddpapi/ddpclient.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "video-conference/videoconferencecanceljob.h"
#include "video-conference/videoconferencejoinjob.h"
#include "video-conference/videoconferencestartjob.h"
#include "videoconference/videoconferencemanager.h"

#include <KLocalizedString>
#include <QDesktopServices>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>

ConferenceDirectCallDialog::ConferenceDirectCallDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mConferenceCallWidget(new ConferenceCallWidget(account, this))
    , mRocketChatAccount(account)
{
    setWindowTitle(i18nc("@title:window", "Start a Direct Call"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mConferenceCallWidget->setObjectName(QStringLiteral("mConferenceCallWidget"));
    mainLayout->addWidget(mConferenceCallWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mOkButton = button->button(QDialogButtonBox::Ok);
    mOkButton->setText(i18n("Start"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ConferenceDirectCallDialog::slotRejected);
    connect(button, &QDialogButtonBox::accepted, this, &ConferenceDirectCallDialog::slotStartVideoConference);
}

ConferenceDirectCallDialog::~ConferenceDirectCallDialog() = default;

void ConferenceDirectCallDialog::slotRejected()
{
    auto job = new RocketChatRestApi::VideoConferenceCancelJob(this);
    job->setCallId(mCallId);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::VideoConferenceCancelJob::videoConferenceCancelDone, this, [](const QJsonObject &obj) {
        qDebug() << "stop obj " << obj;
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start VideoConferenceCancelJob job";
    }
    reject();
}

void ConferenceDirectCallDialog::slotStartVideoConference()
{
    // Disable start button
    mOkButton->setEnabled(false);
    auto job = new RocketChatRestApi::VideoConferenceStartJob(this);
    RocketChatRestApi::VideoConferenceStartJob::VideoConferenceStartInfo startInfo;
    startInfo.roomId = mRoomId;
    startInfo.allowRinging = mAllowRinging;
    job->setInfo(startInfo);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::VideoConferenceStartJob::videoConferenceStartDone, this, [this](const QJsonObject &obj) {
        mCallId = obj[QLatin1String("callId")].toString();
        callUser();
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start VideoConferenceStartJob job";
    }
    connect(mRocketChatAccount->videoConferenceManager(),
            &VideoConferenceManager::videoConferenceCallAccepted,
            this,
            &ConferenceDirectCallDialog::slotVideoConferenceAccepted);
    connect(mRocketChatAccount->videoConferenceManager(),
            &VideoConferenceManager::videoConferenceCallCanceled,
            this,
            &ConferenceDirectCallDialog::slotVideoConferenceCanceled);
}

void ConferenceDirectCallDialog::slotVideoConferenceCanceled(const VideoConference &videoConference)
{
    if (videoConference.callId() == mCallId) {
        cancelCall();
        reject();
    }
}

void ConferenceDirectCallDialog::slotVideoConferenceAccepted(const VideoConference &videoConference)
{
    if (videoConference.callId() == mCallId) {
        // qDebug() << " videoConference " << videoConference;
        mRocketChatAccount->ddp()->videoConferenceConfirmed(mRoomId, mCallId, mRocketChatAccount->userId());
        mWasAccepted = true;
        auto conferenceJoinJob = new RocketChatRestApi::VideoConferenceJoinJob(this);
        RocketChatRestApi::VideoConferenceJoinJob::VideoConferenceJoinInfo joinInfo;
        joinInfo.callId = mCallId;
        const auto confCallInfo = mConferenceCallWidget->startInfo();
        joinInfo.useCamera = confCallInfo.useCamera;
        joinInfo.useMicro = confCallInfo.useMic;
        conferenceJoinJob->setInfo(joinInfo);
        mRocketChatAccount->restApi()->initializeRestApiJob(conferenceJoinJob);
        connect(conferenceJoinJob, &RocketChatRestApi::VideoConferenceJoinJob::videoConferenceJoinDone, this, [this](const QJsonObject &obj) {
            // qDebug() << " join info " << obj;
            QDesktopServices::openUrl(QUrl(obj[QLatin1String("url")].toString()));
            accept();
        });
        if (!conferenceJoinJob->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start VideoConferenceJoinJob job";
        }
    }
}

void ConferenceDirectCallDialog::callUser()
{
    if (mWasAccepted) {
        return;
    }
    mRocketChatAccount->ddp()->videoConferenceCall(mRoomId, mCallId, mRocketChatAccount->userId());
    if (mNumberOfCall <= 3) {
        QTimer::singleShot(3000, this, &ConferenceDirectCallDialog::callUser);
    } else {
        cancelCall();
        reject();
    }
    mNumberOfCall++;
}

void ConferenceDirectCallDialog::cancelCall()
{
    mRocketChatAccount->ddp()->videoConferenceRejected(mRoomId, mCallId, mRocketChatAccount->userId());
}

bool ConferenceDirectCallDialog::allowRinging() const
{
    return mAllowRinging;
}

void ConferenceDirectCallDialog::setAllowRinging(bool newAllowRinging)
{
    mAllowRinging = newAllowRinging;
}

QString ConferenceDirectCallDialog::roomId() const
{
    return mRoomId;
}

void ConferenceDirectCallDialog::setRoomId(const QString &newRoomId)
{
    mRoomId = newRoomId;
}
