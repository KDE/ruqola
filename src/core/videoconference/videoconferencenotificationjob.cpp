/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencenotificationjob.h"
#include "rocketchataccount.h"
#include "ruqola_debug.h"
#include <KLocalizedString>
#include <KNotification>

VideoConferenceNotificationJob::VideoConferenceNotificationJob(QObject *parent)
    : QObject{parent}
{
}

VideoConferenceNotificationJob::~VideoConferenceNotificationJob() = default;

void VideoConferenceNotificationJob::start()
{
    if (!canStart()) {
        qCWarning(RUQOLA_LOG) << "Impossible to start VideoConferenceNotificationJob";
        deleteLater();
        return;
    }
    // TODO
}

bool VideoConferenceNotificationJob::canStart() const
{
    return mVideoConference.isValid();
}

void VideoConferenceNotificationJob::setRocketChatAccount(RocketChatAccount *account)
{
    mRocketChatAccount = account;
}

VideoConference VideoConferenceNotificationJob::videoConference() const
{
    return mVideoConference;
}

void VideoConferenceNotificationJob::setVideoConference(const VideoConference &newVideoConference)
{
    mVideoConference = newVideoConference;
}
