/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencemessageinfomanager.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "ruqola_videoconference_core_debug.h"
#include "updatevideoconferencemessagejob.h"
#include "video-conference/videoconferenceinfojob.h"
#include <QTimer>
#include <chrono>
using namespace std::chrono_literals;

VideoConferenceMessageInfoManager::VideoConferenceMessageInfoManager(RocketChatAccount *account, QObject *parent)
    : QObject{parent}
    , mRocketChatAccount(account)
    , mTimer(new QTimer(this))
{
    mTimer->setSingleShot(true);
    mTimer->setInterval(500ms);
    connect(mTimer, &QTimer::timeout, this, &VideoConferenceMessageInfoManager::slotUpdateMessage);
}

VideoConferenceMessageInfoManager::~VideoConferenceMessageInfoManager() = default;

RocketChatAccount *VideoConferenceMessageInfoManager::rocketChatAccount() const
{
    return mRocketChatAccount;
}

void VideoConferenceMessageInfoManager::addCallId(const QString &callId)
{
    if (!mCallIdList.contains(callId)) {
        mCallIdList.append(callId);
    }

    if (!mCallIdList.isEmpty()) {
        mTimer->start();
    }
}

void VideoConferenceMessageInfoManager::slotUpdateMessage()
{
    if (!mCallIdList.isEmpty()) {
        const QString callId = mCallIdList.takeFirst();
        updateVideoConferenceInfo(callId);
    }
}

void VideoConferenceMessageInfoManager::updateVideoConferenceInfo(const QString &callId)
{
    auto conferenceInfoJob = new RocketChatRestApi::VideoConferenceInfoJob(this);
    conferenceInfoJob->setCallId(callId);
    mRocketChatAccount->restApi()->initializeRestApiJob(conferenceInfoJob);
    connect(conferenceInfoJob, &RocketChatRestApi::VideoConferenceInfoJob::videoConferenceInfoDone, this, [=](const QJsonObject &videoConfObj) {
        VideoConferenceInfo info;
        info.parse(videoConfObj);
        UpdateVideoConferenceMessageJob *job = new UpdateVideoConferenceMessageJob(this);
        job->setRocketChatAccount(mRocketChatAccount);
        job->setVideoConferenceInfo(info);
        job->start();
    });
    if (!conferenceInfoJob->start()) {
        qCWarning(RUQOLA_VIDEO_CONFERENCE_LOG) << "Impossible to start VideoConferenceInfoJob job";
    }
    if (!mCallIdList.isEmpty()) {
        mTimer->start();
    }
}
