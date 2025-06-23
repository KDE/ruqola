/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferenceinfo.h"

#include "ruqola_videoconference_core_debug.h"
#include "utils.h"

#include <KLocalizedString>

#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
VideoConferenceInfo::VideoConferenceInfo() = default;

VideoConferenceInfo::~VideoConferenceInfo() = default;

void VideoConferenceInfo::parse(const QJsonObject &content)
{
    qCDebug(RUQOLA_VIDEO_CONFERENCE_LOG) << " content " << content;
    mBlockId = content["_id"_L1].toString();
    mStatus = content["status"_L1].toInt();
    mUrl = content["url"_L1].toString();
    mRoomId = content["rid"_L1].toString();
    if (content.contains("createdAt"_L1)) {
        setCreatedAtDateTime(Utils::parseIsoDate(u"createdAt"_s, content));
    }
    if (content.contains("endedAt"_L1)) {
        setEndedAtDateTime(Utils::parseIsoDate(u"endedAt"_s, content));
    }
    const QJsonObject messageObj = content["messages"_L1].toObject();
    mMessageId = messageObj["started"_L1].toString();
    // TODO ended ???

    mConferenceType = convertTypeToEnum(content["type"_L1].toString());
    mProviderName = content["providerName"_L1].toString();
    // Users
    const QJsonArray usersArray = content["users"_L1].toArray();
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
    if (str == "videoconference"_L1) {
        return VideoConferenceInfo::VideoConferenceType::Conference;
    } else if (str == "direct"_L1) {
        return VideoConferenceInfo::VideoConferenceType::Direct;
    }
    qCWarning(RUQOLA_VIDEO_CONFERENCE_LOG) << "VideoConferenceInfo::convertTypeToEnum invalid " << str;
    return VideoConferenceInfo::VideoConferenceType::Unknown;
}

QString VideoConferenceInfo::convertEnumToString(const VideoConferenceInfo &info)
{
    switch (info.conferenceType()) {
    case VideoConferenceInfo::VideoConferenceType::Conference:
        return u"videoconference"_s;
    case VideoConferenceInfo::VideoConferenceType::Direct:
        return u"direct"_s;
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
    obj["_id"_L1] = videoConfInfo.mBlockId;
    obj["status"_L1] = videoConfInfo.mStatus;
    obj["url"_L1] = videoConfInfo.mUrl;
    obj["rid"_L1] = videoConfInfo.mRoomId;
    obj["providerName"_L1] = videoConfInfo.mProviderName;
    obj["messageId"_L1] = videoConfInfo.mMessageId;
    obj["type"_L1] = VideoConferenceInfo::convertEnumToString(videoConfInfo);
    obj["createdAt"_L1] = videoConfInfo.createdAtDateTime();
    obj["endedAt"_L1] = videoConfInfo.endedAtDateTime();
    if (!videoConfInfo.mUsers.isEmpty()) {
        QJsonArray userArray;
        for (const User &user : videoConfInfo.mUsers) {
            userArray.append(User::serialize(user));
        }
        obj["users"_L1] = userArray;
    }
    return obj;
}

VideoConferenceInfo VideoConferenceInfo::deserialize(const QJsonObject &o)
{
    VideoConferenceInfo info;
    info.mBlockId = o["_id"_L1].toString();
    info.mStatus = o["status"_L1].toInt();
    info.mUrl = o["url"_L1].toString();
    info.mRoomId = o["rid"_L1].toString();
    info.mProviderName = o["providerName"_L1].toString();
    info.mMessageId = o["messageId"_L1].toString();
    info.mCreatedAtDateTime = o["createdAt"_L1].toInt();
    info.mEndedAtDateTime = o["endedAt"_L1].toInt();

    info.mConferenceType = info.convertTypeToEnum(o["type"_L1].toString());
    const QJsonArray usersArray = o["users"_L1].toArray();
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

QList<User> VideoConferenceInfo::users() const
{
    return mUsers;
}

void VideoConferenceInfo::setUsers(const QList<User> &newUsers)
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
    d.space() << "mUrl " << t.url();
    d.space() << "mStatus " << t.status();
    d.space() << "mRinging " << t.ringing();
    d.space() << "mRoomId " << t.roomId();
    d.space() << "mCreatedAtDateTime " << t.createdAtDateTime();
    d.space() << "mEndedAtDateTime " << t.endedAtDateTime();
    d.space() << "mProviderName " << t.providerName();
    d.space() << "mUsers " << t.users();
    d.space() << "mConferenceType " << t.conferenceType();
    d.space() << "mMessageId " << t.messageId();
    d.space() << "mBlockId " << t.blockId();
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

#include "moc_videoconferenceinfo.cpp"
