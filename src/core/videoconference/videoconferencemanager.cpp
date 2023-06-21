/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencemanager.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "ruqola_videoconference_core_debug.h"
#include "video-conference/videoconferencejoinjob.h"
#include "videoconferencenotificationjob.h"
#include <QDesktopServices>
#include <QJsonObject>

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
                mVideoConferenceList.append(std::move(videoConference));

                switch (videoConference.action()) {
                case VideoConference::Unknown:
                    break;
                case VideoConference::IncomingCall:
                    showNotification(videoConference);
                    break;
                case VideoConference::Accepted:
                    Q_EMIT videoConferenceCallAccepted(videoConference);
                    break;
                case VideoConference::Canceled:
                    Q_EMIT videoConferenceCallCanceled(videoConference);
                    break;
                case VideoConference::Confirmed:
                    Q_EMIT videoConferenceCallConfirmed(videoConference);
                    break;
                case VideoConference::Rejected:
                    Q_EMIT videoConferenceCallRejected(videoConference);
                    break;
                }
            }
        }
    }
}

void VideoConferenceManager::showNotification(const VideoConference &videoConference)
{
    auto job = new VideoConferenceNotificationJob(this);
    connect(job, &VideoConferenceNotificationJob::acceptVideoConference, this, [this, videoConference]() {
        // Send info about accept video conf.
        mRocketChatAccount->ddp()->videoConferenceAccepted(videoConference.roomId(), videoConference.callId(), mRocketChatAccount->userId());

        auto conferenceJoinJob = new RocketChatRestApi::VideoConferenceJoinJob(this);
        RocketChatRestApi::VideoConferenceJoinJob::VideoConferenceJoinInfo joinInfo;
        QJsonObject obj;
        joinInfo.callId = videoConference.callId();
        // TODO joinInfo.useCamera = callInfo.useCamera;
        // TODO joinInfo.useMicro = callInfo.useMic;
        conferenceJoinJob->setInfo(joinInfo);
        mRocketChatAccount->restApi()->initializeRestApiJob(conferenceJoinJob);
        connect(conferenceJoinJob, &RocketChatRestApi::VideoConferenceJoinJob::videoConferenceJoinDone, this, [videoConference, this](const QJsonObject &obj) {
            // qDebug() << " join info " << obj;
            QDesktopServices::openUrl(QUrl(obj[QLatin1String("url")].toString()));
            mVideoConferenceList.removeAll(videoConference);
        });
        if (!conferenceJoinJob->start()) {
            qCWarning(RUQOLA_VIDEO_CONFERENCE_LOG) << "Impossible to start VideoConferenceJoinJob job";
        }
    });
    connect(job, &VideoConferenceNotificationJob::rejectVideoConference, this, [this, videoConference]() {
        // Send info about reject video conf.
        mRocketChatAccount->ddp()->videoConferenceRejected(videoConference.roomId(), videoConference.callId(), mRocketChatAccount->userId());
        mVideoConferenceList.removeAll(videoConference);
    });
    job->setRocketChatAccount(mRocketChatAccount);
    job->setVideoConference(videoConference);
    job->start();
}

#include "moc_videoconferencemanager.cpp"
