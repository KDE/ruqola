/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencemanager.h"

#include "connection.h"
#include "ddpapi/ddpclient.h"
#include "rocketchataccount.h"
#include "ruqola_videoconference_core_debug.h"
#include "ruqolautils.h"
#include "video-conference/videoconferencejoinjob.h"
#include "videoconferencenotificationjob.h"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
VideoConferenceManager::VideoConferenceManager(RocketChatAccount *account, QObject *parent)
    : QObject{parent}
    , mRocketChatAccount(account)
{
}

VideoConferenceManager::~VideoConferenceManager() = default;

void VideoConferenceManager::parseVideoConference(const QJsonArray &contents)
{
    for (const QJsonValue &current : contents) {
        const QJsonObject videoConfObject = current.toObject();
        VideoConference videoConference;
        videoConference.parseVideoConference(videoConfObject);
        if (videoConference.isValid()) {
            if (!mVideoConferenceList.contains(videoConference)) {
                switch (videoConference.action()) {
                case VideoConference::Action::Unknown:
                    break;
                case VideoConference::Action::IncomingCall:
                    showNotification(videoConference);
                    break;
                case VideoConference::Action::Accepted:
                    Q_EMIT videoConferenceCallAccepted(videoConference);
                    break;
                case VideoConference::Action::Canceled:
                    Q_EMIT videoConferenceCallCanceled(videoConference);
                    break;
                case VideoConference::Action::Confirmed:
                    Q_EMIT videoConferenceCallConfirmed(videoConference);
                    break;
                case VideoConference::Action::Rejected:
                    Q_EMIT videoConferenceCallRejected(videoConference);
                    break;
                case VideoConference::Action::Ring:
                    // TODO
                    qCWarning(RUQOLA_VIDEO_CONFERENCE_LOG) << " Ring not implemented yet";
                    break;
                }
                mVideoConferenceList.append(std::move(videoConference));
            }
        }
    }
}

void VideoConferenceManager::showNotification(const VideoConference &videoConference)
{
    auto job = new VideoConferenceNotificationJob(this);
    connect(job, &VideoConferenceNotificationJob::acceptVideoConference, this, [this, videoConference]() {
        // Send info about accept video conf.
        mRocketChatAccount->ddp()->videoConferenceAccepted(QString::fromLatin1(videoConference.roomId()),
                                                           QString::fromLatin1(videoConference.callId()),
                                                           QString::fromLatin1(mRocketChatAccount->userId()));

        auto conferenceJoinJob = new RocketChatRestApi::VideoConferenceJoinJob(this);
        RocketChatRestApi::VideoConferenceJoinJob::VideoConferenceJoinInfo joinInfo;
        QJsonObject obj;
        joinInfo.callId = QString::fromLatin1(videoConference.callId());
        // TODO joinInfo.useCamera = callInfo.useCamera;
        // TODO joinInfo.useMicro = callInfo.useMic;
        conferenceJoinJob->setInfo(joinInfo);
        mRocketChatAccount->restApi()->initializeRestApiJob(conferenceJoinJob);
        connect(conferenceJoinJob, &RocketChatRestApi::VideoConferenceJoinJob::videoConferenceJoinDone, this, [videoConference, this](const QJsonObject &obj) {
            // qDebug() << " join info " << obj;
            RuqolaUtils::self()->openUrl(QUrl(obj["url"_L1].toString()));
            mVideoConferenceList.removeAll(videoConference);
        });
        if (!conferenceJoinJob->start()) {
            qCWarning(RUQOLA_VIDEO_CONFERENCE_LOG) << "Impossible to start VideoConferenceJoinJob job";
        }
    });
    connect(job, &VideoConferenceNotificationJob::rejectVideoConference, this, [this, videoConference]() {
        // Send info about reject video conf.
        mRocketChatAccount->ddp()->videoConferenceRejected(QString::fromLatin1(videoConference.roomId()),
                                                           QString::fromLatin1(videoConference.callId()),
                                                           QString::fromLatin1(mRocketChatAccount->userId()));
        mVideoConferenceList.removeAll(videoConference);
    });
    job->setRocketChatAccount(mRocketChatAccount);
    job->setVideoConference(videoConference);
    job->start();
}

#include "moc_videoconferencemanager.cpp"
