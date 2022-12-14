/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferenceinfo.h"

VideoConferenceInfo::VideoConferenceInfo() = default;

VideoConferenceInfo::~VideoConferenceInfo() = default;

void VideoConferenceInfo::parse(const QJsonObject &content)
{
    // TODO
}

QString VideoConferenceInfo::url() const
{
    return mUrl;
}

void VideoConferenceInfo::setUrl(const QString &newUrl)
{
    mUrl = newUrl;
}

int VideoConferenceInfo::status() const
{
    return mStatus;
}

void VideoConferenceInfo::setStatus(int newStatus)
{
    mStatus = newStatus;
}

bool VideoConferenceInfo::ringing() const
{
    return mRinging;
}

void VideoConferenceInfo::setRinging(bool newRinging)
{
    mRinging = newRinging;
}

QString VideoConferenceInfo::roomId() const
{
    return mRoomId;
}

void VideoConferenceInfo::setRoomId(const QString &newRoomId)
{
    mRoomId = newRoomId;
}

QDebug operator<<(QDebug d, const VideoConferenceInfo &t)
{
    d << "mUrl " << t.url();
    d << "mStatus " << t.status();
    d << "mRinging " << t.ringing();
    d << "mRoomId " << t.roomId();
    return d;
}
