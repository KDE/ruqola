/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferenceinfo.h"
#include "ruqola_videoconference_core_debug.h"
#include "utils.h"

#include <KLocalizedString>

#include <QJsonObject>

VideoConferenceInfo::VideoConferenceInfo() = default;

VideoConferenceInfo::~VideoConferenceInfo() = default;

void VideoConferenceInfo::parse(const QJsonObject &content)
{
    qCDebug(RUQOLA_VIDEO_CONFERENCE_LOG) << " content " << content;
    mBlockId = content[QLatin1String("_id")].toString();
    mStatus = content[QLatin1String("status")].toInt();
    mUrl = content[QLatin1String("url")].toString();
    mRoomId = content[QLatin1String("rid")].toString();
    if (content.contains(QLatin1String("createdAt"))) {
        setCreatedAtDateTime(Utils::parseIsoDate(QStringLiteral("createdAt"), content));
    }
    if (content.contains(QLatin1String("endedAt"))) {
        setEndedAtDateTime(Utils::parseIsoDate(QStringLiteral("endedAt"), content));
    }
    const QJsonObject messageObj = content[QLatin1String("messages")].toObject();
    mMessageId = messageObj[QLatin1String("started")].toString();
    // TODO ended ???

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

QString VideoConferenceInfo::convertEnumToString(const VideoConferenceInfo &info)
{
    switch (info.conferenceType()) {
    case VideoConferenceInfo::VideoConferenceType::Conference:
        return QStringLiteral("videoconference");
    case VideoConferenceInfo::VideoConferenceType::Direct:
        return QStringLiteral("direct");
    case VideoConferenceInfo::VideoConferenceType::Unknown:
        return {};
    }
    qCWarning(RUQOLA_VIDEO_CONFERENCE_LOG) << "VideoConferenceInfo::convertEnumToString invalid ";
    return {};
}

QString VideoConferenceInfo::blockId() const
{
    return mBlockId;
}

void VideoConferenceInfo::setBlockId(const QString &newBlockId)
{
    mBlockId = newBlockId;
}

bool VideoConferenceInfo::isValid() const
{
    return conferenceType() != VideoConferenceInfo::VideoConferenceType::Unknown;
}

bool VideoConferenceInfo::canJoin() const
{
    return createdAtDateTime() != -1 && endedAtDateTime() == -1;
}

QString VideoConferenceInfo::title() const
{
    return statusInformation();
}

QJsonObject VideoConferenceInfo::serialize(const VideoConferenceInfo &videoConfInfo)
{
    QJsonObject obj;
    obj[QLatin1String("_id")] = videoConfInfo.mBlockId;
    obj[QLatin1String("status")] = videoConfInfo.mStatus;
    obj[QLatin1String("url")] = videoConfInfo.mUrl;
    obj[QLatin1String("rid")] = videoConfInfo.mRoomId;
    obj[QLatin1String("providerName")] = videoConfInfo.mProviderName;
    obj[QLatin1String("messageId")] = videoConfInfo.mMessageId;
    obj[QLatin1String("type")] = VideoConferenceInfo::convertEnumToString(videoConfInfo);
    // TODO createdAt/endedAt ?
    if (!videoConfInfo.mUsers.isEmpty()) {
        QJsonArray userArray;
        for (const User &user : videoConfInfo.mUsers) {
            userArray.append(User::serialize(user));
        }
        obj[QLatin1String("users")] = userArray;
    }
    return obj;
}

VideoConferenceInfo VideoConferenceInfo::deserialize(const QJsonObject &o)
{
    VideoConferenceInfo info;
    info.mBlockId = o[QLatin1String("_id")].toString();
    info.mStatus = o[QLatin1String("status")].toInt();
    info.mUrl = o[QLatin1String("url")].toString();
    info.mRoomId = o[QLatin1String("rid")].toString();
    info.mProviderName = o[QLatin1String("providerName")].toString();
    info.mMessageId = o[QLatin1String("messageId")].toString();
    // TODO createdAt/endedAt ?

    info.mConferenceType = info.convertTypeToEnum(o[QLatin1String("type")].toString());
    const QJsonArray usersArray = o[QLatin1String("users")].toArray();
    info.mUsers.reserve(usersArray.count());
    for (const QJsonValue &current : usersArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject userObject = current.toObject();
            User m;
            m.parseUserRestApi(userObject, {});
            if (m.isValid()) {
                info.mUsers.append(std::move(m));
            }
        } else {
            qCWarning(RUQOLA_VIDEO_CONFERENCE_LOG) << "Problem when parsing Users" << current;
        }
    }
    return info;
}

QString VideoConferenceInfo::messageId() const
{
    return mMessageId;
}

void VideoConferenceInfo::setMessageId(const QString &newMessageId)
{
    mMessageId = newMessageId;
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

qint64 VideoConferenceInfo::createdAtDateTime() const
{
    return mCreatedAtDateTime;
}

void VideoConferenceInfo::setCreatedAtDateTime(qint64 newCreatedAtDateTime)
{
    mCreatedAtDateTime = newCreatedAtDateTime;
}

qint64 VideoConferenceInfo::endedAtDateTime() const
{
    return mEndedAtDateTime;
}

void VideoConferenceInfo::setEndedAtDateTime(qint64 newEndedAtDateTime)
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
    d << "mConferenceType " << t.conferenceType();
    d << "mMessageId " << t.messageId();
    d << "mBlockId " << t.blockId();
    return d;
}

bool VideoConferenceInfo::operator==(const VideoConferenceInfo &other) const
{
    return mCreatedAtDateTime == other.createdAtDateTime() && mEndedAtDateTime == other.endedAtDateTime() && mUrl == other.url() && mRoomId == other.roomId()
        && mProviderName == other.providerName() && mConferenceType == other.conferenceType() && mStatus == other.status() && mRinging == other.ringing()
        && mUsers == other.users() && mMessageId == other.messageId() && mBlockId == other.blockId();
}

QString VideoConferenceInfo::statusInformation() const
{
    if (endedAtDateTime() >= 0) {
        if (conferenceType() == VideoConferenceInfo::VideoConferenceType::Direct) {
            return i18n("Call was not answered");
        } else if (conferenceType() == VideoConferenceInfo::VideoConferenceType::Conference && users().isEmpty()) {
            return i18n("Call was not answered");
        } else {
            return i18n("Call ended");
        }
    } else {
        if (conferenceType() == VideoConferenceInfo::VideoConferenceType::Direct && status() == 0) {
            return i18n("Waiting for answer");
        } else {
            return i18n("Call ongoing");
        }
    }
    return {};
}
