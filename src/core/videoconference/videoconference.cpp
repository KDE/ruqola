/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconference.h"

#include "ruqola_debug.h"

#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
VideoConference::VideoConference() = default;

VideoConference::~VideoConference() = default;

bool VideoConference::operator==(const VideoConference &other) const
{
    return mAction == other.action() && mCallId == other.callId() && mRoomId == other.roomId() && mUserId == other.userId();
}

void VideoConference::parseVideoConference(const QJsonObject &content)
{
    mAction = convertActionToEnum(content["action"_L1].toString());
    const QJsonObject videoConfParams = content["params"_L1].toObject();
    mCallId = videoConfParams["callId"_L1].toString().toLatin1();
    mRoomId = videoConfParams["rid"_L1].toString().toLatin1();
    mUserId = videoConfParams["uid"_L1].toString().toLatin1();
    // {"action":"call","params":{"callId":"63983180a7f9e1466a4eedc6","rid":"YbwG4T2uB3wZSZSKBxkNpoB3T98EEPCj2K","uid":"YbwG4T2uB3wZSZSKB"}}
}

VideoConference::Action VideoConference::convertActionToEnum(const QString &str)
{
    Action action = Action::Unknown;
    if (str == "call"_L1) {
        action = Action::IncomingCall;
    } else if (str == "canceled"_L1) {
        action = Action::Canceled;
    } else if (str == "confirmed"_L1) {
        action = Action::Confirmed;
    } else if (str == "accepted"_L1) {
        action = Action::Accepted;
    } else if (str == "rejected"_L1) {
        action = Action::Rejected;
    } else if (str == "ring"_L1) {
        action = Action::Ring;
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

QByteArray VideoConference::callId() const
{
    return mCallId;
}

void VideoConference::setCallId(const QByteArray &newCallId)
{
    mCallId = newCallId;
}

QByteArray VideoConference::roomId() const
{
    return mRoomId;
}

void VideoConference::setRoomId(const QByteArray &newRoomId)
{
    mRoomId = newRoomId;
}

QByteArray VideoConference::userId() const
{
    return mUserId;
}

void VideoConference::setUserId(const QByteArray &newUserId)
{
    mUserId = newUserId;
}

bool VideoConference::isValid() const
{
    return !mCallId.isEmpty() && !mRoomId.isEmpty() && !mUserId.isEmpty() && mAction != Action::Unknown;
}

QDebug operator<<(QDebug d, const VideoConference &t)
{
    d.space() << "mCallId " << t.callId();
    d.space() << "mRoomId " << t.roomId();
    d.space() << "mUserId " << t.userId();
    d.space() << "mAction " << t.action();
    return d;
}

#include "moc_videoconference.cpp"
