/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roominfo/roominfo.h"

#include "ruqola_debug.h"
#include "utils.h"
#include <KLocalizedString>
#include <QJsonArray>
#include <QLocale>

RoomInfo::RoomInfo() = default;

RoomInfo::~RoomInfo() = default;

void RoomInfo::parseRoomInfo(const QJsonObject &object)
{
    // qDebug() << " void AdminRoom::parseAdminRoom(const QJsonObject &object)" << object;
    if (object.contains(QLatin1String("name"))) {
        setName(object[QStringLiteral("name")].toString());
    }
    setTopic(object[QStringLiteral("topic")].toString());
    setDescription(object[QStringLiteral("description")].toString());
    setAnnouncement(object[QStringLiteral("announcement")].toString());
    if (object.contains(QLatin1String("msgs"))) {
        setMessageCount(object[QStringLiteral("msgs")].toInt());
    }
    if (object.contains(QLatin1String("t"))) {
        setChannelType(object[QStringLiteral("t")].toString());
    }
    setDefaultRoom(object[QStringLiteral("default")].toBool(false));
    if (object.contains(QLatin1String("lastMessage"))) {
        setLastMessage(Utils::parseIsoDate(QStringLiteral("_updatedAt"), object[QStringLiteral("lastMessage")].toObject()));
    }
    if (object.contains(QLatin1String("ts"))) {
        setCreatedRoom(Utils::parseIsoDate(QStringLiteral("ts"), object));
    }
    setIdentifier(object[QStringLiteral("_id")].toString());
    setReadOnly(object[QStringLiteral("ro")].toBool());

    setFeatured(object[QStringLiteral("featured")].toBool(false));
    if (object.contains(QLatin1String("usersCount"))) {
        setUsersCount(object[QStringLiteral("usersCount")].toInt());
    }
    const QJsonArray userNamesArray = object[QStringLiteral("usernames")].toArray();
    const QVariantList userNamesList = userNamesArray.toVariantList();
    mUserNames.reserve(userNamesList.count());
    for (const auto &var : userNamesList) {
        mUserNames.append(var.toString());
    }
    mTeamInfo.parseTeamInfo(object);
    generateDisplayChannelType();

    const QJsonValue ownerValue = object.value(QLatin1String("u"));
    if (!ownerValue.isUndefined()) {
        const QJsonObject objOwner = ownerValue.toObject();
        mOwnerName = objOwner.value(QLatin1String("username")).toString();
    }

    // TODO load team info
    // qDebug() << " * this " << *this;
    // Add users "u"
    // TODO
}

bool RoomInfo::defaultRoom() const
{
    return mDefaultRoom;
}

void RoomInfo::setDefaultRoom(bool defaultRoom)
{
    mDefaultRoom = defaultRoom;
}

int RoomInfo::usersCount() const
{
    return mUsersCount;
}

void RoomInfo::setUsersCount(int usersCount)
{
    mUsersCount = usersCount;
}

int RoomInfo::messageCount() const
{
    return mMessageCount;
}

void RoomInfo::setMessageCount(int messageCount)
{
    mMessageCount = messageCount;
}

QString RoomInfo::channelType() const
{
    return mChannelType;
}

static QString convertChannelType(const QString &str, bool mainTeam)
{
    if (str == QLatin1Char('p')) {
        if (mainTeam) {
            return i18n("Private Team");
        } else {
            return i18n("Group");
        }
    } else if (str == QLatin1Char('c')) {
        if (mainTeam) {
            return i18n("Public Team");
        } else {
            return i18n("Channel");
        }
    } else if (str == QLatin1Char('d')) {
        return i18n("Direct");
    } else if (str == QLatin1Char('l')) {
        return i18n("Omnichannel");
    } else {
        qCWarning(RUQOLA_LOG) << " Unkwnon channel type " << str;
        return str;
    }
}

bool RoomInfo::isTeam() const
{
    return mTeamInfo.mainTeam();
}

void RoomInfo::generateDisplayChannelType()
{
    mChannelTypeStr = convertChannelType(mChannelType, mTeamInfo.mainTeam());
}

const QString &RoomInfo::ownerName() const
{
    return mOwnerName;
}

void RoomInfo::setOwnerName(const QString &newOwnerName)
{
    mOwnerName = newOwnerName;
}

const QString &RoomInfo::announcement() const
{
    return mAnnouncement;
}

void RoomInfo::setAnnouncement(const QString &newAnnouncement)
{
    mAnnouncement = newAnnouncement;
}

const QString &RoomInfo::description() const
{
    return mDescription;
}

void RoomInfo::setDescription(const QString &newDescription)
{
    mDescription = newDescription;
}

bool RoomInfo::featured() const
{
    return mFeatured;
}

void RoomInfo::setFeatured(bool newFeatured)
{
    mFeatured = newFeatured;
}

qint64 RoomInfo::createdRoom() const
{
    return mCreatedRoom;
}

void RoomInfo::setCreatedRoom(qint64 newCreatedRoom)
{
    mCreatedRoom = newCreatedRoom;
    if (mCreatedRoom != -1) {
        QLocale l;
        mCreatedRoomDisplayTime = l.toString(QDateTime::fromMSecsSinceEpoch(mCreatedRoom), QLocale::LongFormat);
    }
}

QString RoomInfo::createdRoomDisplayDateTimeStr() const
{
    return mCreatedRoomDisplayTime;
}

qint64 RoomInfo::lastMessage() const
{
    return mLastMessage;
}

QString RoomInfo::lastMessageDisplayDateTimeStr() const
{
    return mLastMessageDisplayTime;
}

void RoomInfo::setLastMessage(qint64 newLastMessage)
{
    mLastMessage = newLastMessage;
    if (mLastMessage != -1) {
        QLocale l;
        mLastMessageDisplayTime = l.toString(QDateTime::fromMSecsSinceEpoch(mLastMessage), QLocale::LongFormat);
    }
}

void RoomInfo::setChannelType(const QString &channelType)
{
    mChannelType = channelType;
}

QString RoomInfo::identifier() const
{
    return mIdentifier;
}

void RoomInfo::setIdentifier(const QString &identifier)
{
    mIdentifier = identifier;
}

QString RoomInfo::topic() const
{
    return mTopic;
}

void RoomInfo::setTopic(const QString &topic)
{
    mTopic = topic;
}

bool RoomInfo::readOnly() const
{
    return mReadOnly;
}

void RoomInfo::setReadOnly(bool readOnly)
{
    mReadOnly = readOnly;
}

QString RoomInfo::roomName() const
{
    if (mName.isEmpty()) {
        return mUserNames.join(QStringLiteral(" x "));
    }
    return mName;
}

const TeamInfo &RoomInfo::teamInfo() const
{
    return mTeamInfo;
}

void RoomInfo::setTeamInfo(const TeamInfo &newTeamInfo)
{
    mTeamInfo = newTeamInfo;
}

QString RoomInfo::name() const
{
    return mName;
}

void RoomInfo::setName(const QString &name)
{
    mName = name;
}

QStringList RoomInfo::userNames() const
{
    return mUserNames;
}

void RoomInfo::setUserNames(const QStringList &userNames)
{
    mUserNames = userNames;
}

QStringList RoomInfo::users() const
{
    return mUsers;
}

void RoomInfo::setUsers(const QStringList &users)
{
    mUsers = users;
}

QString RoomInfo::channelTypeStr() const
{
    return mChannelTypeStr;
}

bool RoomInfo::operator==(const RoomInfo &other) const
{
    return mDefaultRoom == other.defaultRoom() && mUsersCount == other.usersCount() && mMessageCount == other.messageCount()
        && mChannelType == other.channelType() && mIdentifier == other.identifier() && mTopic == other.topic() && mName == other.name()
        && mUserNames == other.userNames() && mUsers == other.users() && mTeamInfo == other.teamInfo() && mLastMessage == other.lastMessage()
        && mCreatedRoom == other.createdRoom() && mDescription == other.description() && mAnnouncement == other.announcement()
        && mOwnerName == other.ownerName();
}

QDebug operator<<(QDebug d, const RoomInfo &t)
{
    d << " default Room : " << t.defaultRoom();
    d << " user count : " << t.usersCount();
    d << " message count : " << t.messageCount();
    d << " channel type: " << t.channelType();
    d << " identifier: " << t.identifier();
    d << " topic: " << t.topic();
    d << " name: " << t.name();
    d << " usernames: " << t.userNames();
    d << " users: " << t.users();
    d << " teaminfo: " << t.teamInfo();
    d << " lastMessage : " << t.lastMessage();
    d << " created : " << t.createdRoom();
    d << " description : " << t.description();
    d << " announcement : " << t.announcement();
    d << " OwnerName : " << t.ownerName();
    return d;
}
