/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconference.h"
#include "ruqola_debug.h"

#include <QJsonObject>

VideoConference::VideoConference() = default;

VideoConference::~VideoConference() = default;

bool VideoConference::operator==(const VideoConference &other) const
{
    return mAction == other.action() && mCallId == other.callId() && mRoomId == other.roomId() && mUserId == other.userId();
}

void VideoConference::parseVideoConference(const QJsonObject &content)
{
    mAction = convertActionToEnum(content[QLatin1String("action")].toString());
    const QJsonObject videoConfParams = content[QLatin1String("params")].toObject();
    mCallId = videoConfParams[QLatin1String("callId")].toString();
    mRoomId = videoConfParams[QLatin1String("rid")].toString();
    mUserId = videoConfParams[QLatin1String("uid")].toString();
    // {"action":"call","params":{"callId":"63983180a7f9e1466a4eedc6","rid":"YbwG4T2uB3wZSZSKBxkNpoB3T98EEPCj2K","uid":"YbwG4T2uB3wZSZSKB"}}
}

VideoConference::Action VideoConference::convertActionToEnum(const QString &str)
{
    Action action = Unknown;
    if (str == QLatin1String("call")) {
        action = IncomingCall;
    } else if (str == QLatin1String("canceled")) {
        action = Canceled;
    } else if (str == QLatin1String("confirmed")) {
        action = Confirmed;
    } else if (str == QLatin1String("accepted")) {
        action = Accepted;
    } else if (str == QLatin1String("rejected")) {
        action = Rejected;
    } else {
        qCWarning(RUQOLA_LOG) << "Action not implemented! " << str;
    }
    return action;
}

VideoConference::Action VideoConference::action() const
{
    return mAction;
}

void VideoConference::setAction(Action newAction)
{
    mAction = newAction;
}

QString VideoConference::callId() const
{
    return mCallId;
}

void VideoConference::setCallId(const QString &newCallId)
{
    mCallId = newCallId;
}

QString VideoConference::roomId() const
{
    return mRoomId;
}

void VideoConference::setRoomId(const QString &newRoomId)
{
    mRoomId = newRoomId;
}

QString VideoConference::userId() const
{
    return mUserId;
}

void VideoConference::setUserId(const QString &newUserId)
{
    mUserId = newUserId;
}

bool VideoConference::isValid() const
{
    return !mCallId.isEmpty() && !mRoomId.isEmpty() && !mUserId.isEmpty() && mAction != Unknown;
}

QDebug operator<<(QDebug d, const VideoConference &t)
{
    d << "mCallId " << t.callId();
    d << "mRoomId " << t.roomId();
    d << "mUserId " << t.userId();
    d << "mAction " << t.action();
    return d;
}
