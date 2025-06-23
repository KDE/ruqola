/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roominfo/roominfo.h"

#include "ruqola_debug.h"
#include "utils.h"
#include <KLocalizedString>
#include <QJsonArray>
#include <QLocale>

using namespace Qt::Literals::StringLiterals;
RoomInfo::RoomInfo() = default;

RoomInfo::~RoomInfo() = default;

void RoomInfo::parseRoomInfo(const QJsonObject &object)
{
    // qDebug() << " void AdminRoom::parseAdminRoom(const QJsonObject &object)" << object;
    if (object.contains("name"_L1)) {
        setName(object["name"_L1].toString());
    }

    if (object.contains("fname"_L1)) {
        setFName(object["fname"_L1].toString());
    }

    if (object.contains("prid"_L1)) {
        setParentRid(object["prid"_L1].toString().toLatin1());
    }

    setTopic(object["topic"_L1].toString());
    setDescription(object["description"_L1].toString());
    setAnnouncement(object["announcement"_L1].toString());
    if (object.contains("msgs"_L1)) {
        setMessageCount(object["msgs"_L1].toInt());
    }
    if (object.contains("t"_L1)) {
        setChannelType(object["t"_L1].toString());
    }
    setBelongsTo(object["belongsTo"_L1].toString());
    setDefaultRoom(object["default"_L1].toBool(false));
    if (object.contains("lastMessage"_L1)) {
        setLastMessage(Utils::parseIsoDate(u"_updatedAt"_s, object["lastMessage"_L1].toObject()));
    }
    if (object.contains("ts"_L1)) {
        setCreatedRoom(Utils::parseIsoDate(u"ts"_s, object));
    }
    setIdentifier(object["_id"_L1].toString().toLatin1());
    setReadOnly(object["ro"_L1].toBool());

    setFeatured(object["featured"_L1].toBool(false));
    if (object.contains("usersCount"_L1)) {
        setUsersCount(object["usersCount"_L1].toInt());
    }
    const QJsonArray userNamesArray = object["usernames"_L1].toArray();
    const QVariantList userNamesList = userNamesArray.toVariantList();
    mUserNames.reserve(userNamesList.count());
    for (const auto &var : userNamesList) {
        mUserNames.append(var.toString());
    }
    mTeamInfo.parseTeamInfo(object);
    generateDisplayChannelType();

    const QJsonValue ownerValue = object.value("u"_L1);
    if (!ownerValue.isUndefined()) {
        const QJsonObject objOwner = ownerValue.toObject();
        mOwnerName = objOwner.value("username"_L1).toString();
    }

    // TODO load team info
    // qDebug() << " * this " << *this;
    // Add users "u"
    // TODO
}

void RoomInfo::setParentRid(const QByteArray &rid)
{
    mParentRid = rid;
}

QByteArray RoomInfo::parentRid() const
{
    return mParentRid;
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

static QString convertChannelType(const QString &str, bool mainTeam, bool isDiscussion)
{
    if (isDiscussion) {
        return i18n("Discussion");
    }
    if (str == u'p') {
        if (mainTeam) {
            return i18n("Private Team");
        } else {
            return i18n("Group");
        }
    } else if (str == u'c') {
        if (mainTeam) {
            return i18n("Public Team");
        } else {
            return i18n("Channel");
        }
    } else if (str == u'd') {
        return i18n("Direct message");
    } else if (str == u'l') {
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

bool RoomInfo::isDiscussion() const
{
    return !mParentRid.isEmpty();
}

void RoomInfo::generateDisplayChannelType()
{
    mChannelTypeStr = convertChannelType(mChannelType, isTeam(), !mParentRid.isEmpty());
}

QString RoomInfo::belongsTo() const
{
    return mBelongsTo;
}

void RoomInfo::setBelongsTo(const QString &newBelongsTo)
{
    mBelongsTo = newBelongsTo;
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
        mCreatedRoomDisplayTime = l.toString(QDateTime::fromMSecsSinceEpoch(mCreatedRoom));
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

QByteArray RoomInfo::identifier() const
{
    return mIdentifier;
}

void RoomInfo::setIdentifier(const QByteArray &identifier)
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
    if (!mFName.isEmpty()) {
        return mFName;
    }
    if (mName.isEmpty()) {
        return mUserNames.join(u" x "_s);
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

QString RoomInfo::fName() const
{
    return mFName;
}

void RoomInfo::setFName(const QString &name)
{
    mFName = name;
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
        && mOwnerName == other.ownerName() && mFName == other.fName() && mParentRid == other.parentRid();
}

QDebug operator<<(QDebug d, const RoomInfo &t)
{
    d.space() << "default Room :" << t.defaultRoom();
    d.space() << "user count :" << t.usersCount();
    d.space() << "message count :" << t.messageCount();
    d.space() << "channel type:" << t.channelType();
    d.space() << "identifier:" << t.identifier();
    d.space() << "topic:" << t.topic();
    d.space() << "name:" << t.name();
    d.space() << "fname:" << t.fName();
    d.space() << "usernames:" << t.userNames();
    d.space() << "users:" << t.users();
    d.space() << "teaminfo:" << t.teamInfo();
    d.space() << "lastMessage" << t.lastMessage();
    d.space() << "created:" << t.createdRoom();
    d.space() << "description" << t.description();
    d.space() << "announcement" << t.announcement();
    d.space() << "OwnerName:" << t.ownerName();
    d.space() << "ParentRid:" << t.parentRid();
    return d;
}
