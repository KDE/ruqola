/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferenceinfo.h"
#include "ruqola_videoconference_core_debug.h"
#include "utils.h"

#include <QJsonObject>

VideoConferenceInfo::VideoConferenceInfo() = default;

VideoConferenceInfo::~VideoConferenceInfo() = default;

void VideoConferenceInfo::parse(const QJsonObject &content)
{
    qCDebug(RUQOLA_VIDEO_CONFERENCE_LOG) << " content " << content;
    //{"_id":"63997dd831e2ca494753c544","type":"videoconference","users":[{"_id":"YbwG4T2uB3wZSZSKB","username":"laurent-montel",
    // "name":"laurent","ts":"2022-12-14T07:40:08.973Z"}],"messages":{"started":"ka85EtarYSjjt59w4"},"status":2,
    // "anonymousUsers":0,"createdAt":"2022-12-14T07:40:08.262Z","providerName":"jitsi","ringing":true,"title":"ruqola252",
    // "rid":"kNRwYoAPHKsSjLGkG","createdBy":{"_id":"YbwG4T2uB3wZSZSKB","name":"laurent","username":"laurent-montel"},
    // "_updatedAt":"2022-12-15T09:00:00.050Z","url":"https://jitsi.rocket.chat/RocketChat63997dd831e2ca494753c544","endedAt":"2022-12-15T09:00:00.050Z",
    // "capabilities":{"mic":true,"cam":true,"title":true},"success":true}

    // {"_id":"639b1aba29673367a61eb4f7","type":"direct","users":[{"_id":"xkNpoB3T98EEPCj2K","username":"lili-2","name":"lili","ts":"2022-12-15T13:01:52.043Z"},
    // {"_id":"YbwG4T2uB3wZSZSKB","username":"laurent-montel","name":"laurent","ts":"2022-12-15T13:01:52.473Z"}],"messages":{"started":"qqGWGre8EApENtFZR"},"status":1,
    // "createdAt":"2022-12-15T13:01:46.995Z","providerName":"jitsi","ringing":false,"rid":"YbwG4T2uB3wZSZSKBxkNpoB3T98EEPCj2K",
    // "createdBy":{"_id":"YbwG4T2uB3wZSZSKB","name":"laurent","username":"laurent-montel"},"_updatedAt":"2022-12-15T13:01:52.474Z",
    // "url":"https://jitsi.rocket.chat/RocketChat639b1aba29673367a61eb4f7","capabilities":{"mic":true,"cam":true,"title":true},"success":true}
    mStatus = content[QLatin1String("status")].toInt();
    mUrl = content[QLatin1String("url")].toString();
    if (content.contains(QLatin1String("createdAt"))) {
        setCreatedAtDateTime(QDateTime::fromMSecsSinceEpoch(Utils::parseIsoDate(QStringLiteral("createdAt"), content)));
    }
    if (content.contains(QLatin1String("endedAt"))) {
        setEndedAtDateTime(QDateTime::fromMSecsSinceEpoch(Utils::parseIsoDate(QStringLiteral("endedAt"), content)));
    }
    mConferenceType = convertTypeToEnum(content[QLatin1String("type")].toString());
    mProviderName = content[QLatin1String("providerName")].toString();
    // Users
    const QJsonArray usersArray = content[QLatin1String("users")].toArray();
    mUsers.reserve(usersArray.count());
    for (const QJsonValue &current : usersArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject userObject = current.toObject();
            User m;
            m.parseUserRestApi(userObject, {});
            if (m.isValid()) {
                mUsers.append(std::move(m));
            }
        } else {
            qCWarning(RUQOLA_VIDEO_CONFERENCE_LOG) << "Problem when parsing Users" << current;
        }
    }
}

VideoConferenceInfo::VideoConferenceType VideoConferenceInfo::convertTypeToEnum(const QString &str) const
{
    if (str == QLatin1String("videoconference")) {
        return VideoConferenceInfo::VideoConferenceType::Conference;
    } else if (str == QLatin1String("direct")) {
        return VideoConferenceInfo::VideoConferenceType::Direct;
    }
    qCWarning(RUQOLA_VIDEO_CONFERENCE_LOG) << "VideoConferenceInfo::convertTypeToEnum invalid " << str;
    return VideoConferenceInfo::VideoConferenceType::Unknown;
}

QVector<User> VideoConferenceInfo::users() const
{
    return mUsers;
}

void VideoConferenceInfo::setUsers(const QVector<User> &newUsers)
{
    mUsers = newUsers;
}

QString VideoConferenceInfo::providerName() const
{
    return mProviderName;
}

void VideoConferenceInfo::setProviderName(const QString &newProviderName)
{
    mProviderName = newProviderName;
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

QDateTime VideoConferenceInfo::createdAtDateTime() const
{
    return mCreatedAtDateTime;
}

void VideoConferenceInfo::setCreatedAtDateTime(const QDateTime &newCreatedAtDateTime)
{
    mCreatedAtDateTime = newCreatedAtDateTime;
}

QDateTime VideoConferenceInfo::endedAtDateTime() const
{
    return mEndedAtDateTime;
}

void VideoConferenceInfo::setEndedAtDateTime(const QDateTime &newEndedAtDateTime)
{
    mEndedAtDateTime = newEndedAtDateTime;
}

VideoConferenceInfo::VideoConferenceType VideoConferenceInfo::conferenceType() const
{
    return mConferenceType;
}

void VideoConferenceInfo::setConferenceType(VideoConferenceType newConferenceType)
{
    mConferenceType = newConferenceType;
}

QDebug operator<<(QDebug d, const VideoConferenceInfo &t)
{
    d << "mUrl " << t.url();
    d << "mStatus " << t.status();
    d << "mRinging " << t.ringing();
    d << "mRoomId " << t.roomId();
    d << "mCreatedAtDateTime " << t.createdAtDateTime();
    d << "mEndedAtDateTime " << t.endedAtDateTime();
    d << "mProviderName " << t.providerName();
    d << "mUsers " << t.users();
    return d;
}

bool VideoConferenceInfo::operator==(const VideoConferenceInfo &other) const
{
    return mCreatedAtDateTime == other.createdAtDateTime() && mEndedAtDateTime == other.endedAtDateTime() && mUrl == other.url() && mRoomId == other.roomId()
        && mProviderName == other.providerName() && mConferenceType == other.conferenceType() && mStatus == other.status() && mRinging == other.ringing()
        && mUsers == other.users();
}
