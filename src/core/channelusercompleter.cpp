/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelusercompleter.h"
#include <QJsonObject>

ChannelUserCompleter::ChannelUserCompleter() = default;

ChannelUserCompleter::~ChannelUserCompleter() = default;

QDebug operator<<(QDebug d, const ChannelUserCompleter &t)
{
    d.space() << "Type" << t.type();
    d.space() << "Name" << t.name();
    d.space() << "description" << t.description();
    d.space() << "userName" << t.userName();
    d.space() << "AvatarTag" << t.avatarTag();
    d.space() << "outsideRoom" << t.outsideRoom();
    d.space() << "identifie " << t.identifier();
    d.space() << "avatarinfo" << t.avatarInfo();
    return d;
}

QString ChannelUserCompleter::completerName() const
{
    switch (mType) {
    case ChannelUserCompleterType::DirectChannel:
        return userName();
    case ChannelUserCompleterType::Room:
    case ChannelUserCompleterType::Notification:
        return name();
    case ChannelUserCompleterType::Unknown:
        return {};
    }

    return {};
}

QString ChannelUserCompleter::identifier() const
{
    return mIdentifier;
}

void ChannelUserCompleter::setIdentifier(const QString &newIdentifier)
{
    mIdentifier = newIdentifier;
}

Utils::AvatarInfo ChannelUserCompleter::avatarInfo() const
{
    return mAvatarInfo;
}

void ChannelUserCompleter::parseChannel(const QJsonObject &object, ChannelUserCompleterType type)
{
    // qDebug() << " object " << object;
    mType = type;
    mName = object.value(QLatin1StringView("name")).toString();
    mIdentifier = object.value(QLatin1StringView("_id")).toString();
    if (mType == ChannelUserCompleterType::DirectChannel) {
        mAvatarTag = object.value(QLatin1StringView("avatarETag")).toString();
        mUserName = object.value(QLatin1StringView("username")).toString();
        mStatusIcon = QIcon::fromTheme(Utils::iconFromStatus(object.value(QLatin1StringView("status")).toString()));
    } else {
        const QString roomType = object.value(QLatin1StringView("t")).toString();
        if (roomType == QLatin1Char('c')) {
            setChannelIcon();
        } else if (roomType == QLatin1Char('p')) {
            mStatusIcon = QIcon::fromTheme(QStringLiteral("lock"));
        }
    }
    mOutsideRoom = object.value(QLatin1StringView("outside")).toBool();
    createAvatarInfo();
}

void ChannelUserCompleter::setChannelIcon()
{
    mStatusIcon = QIcon::fromTheme(QStringLiteral("irc-channel-inactive"));
}

void ChannelUserCompleter::createAvatarInfo()
{
    mAvatarInfo.avatarType = (mType == ChannelUserCompleter::ChannelUserCompleterType::Room ? Utils::AvatarType::Room : Utils::AvatarType::User);
    mAvatarInfo.etag = mAvatarTag;
    mAvatarInfo.identifier = (mType == ChannelUserCompleter::ChannelUserCompleterType::Room ? mIdentifier : mUserName);
}

void ChannelUserCompleter::setAvatarInfo(const Utils::AvatarInfo &newAvatarInfo)
{
    mAvatarInfo = newAvatarInfo;
}

bool ChannelUserCompleter::operator==(const ChannelUserCompleter &other) const
{
    return (mType == other.type()) && (mName == other.name())
        && (mDescription == other.description() && (mUserName == other.userName()) && (mAvatarTag == other.avatarTag()) && (mOutsideRoom == other.outsideRoom())
            && (mIdentifier == other.identifier()));
}

QString ChannelUserCompleter::description() const
{
    return mDescription;
}

void ChannelUserCompleter::setDescription(const QString &newDescription)
{
    mDescription = newDescription;
}

QString ChannelUserCompleter::name() const
{
    return mName;
}

void ChannelUserCompleter::setName(const QString &newName)
{
    mName = newName;
}

ChannelUserCompleter::ChannelUserCompleterType ChannelUserCompleter::type() const
{
    return mType;
}

void ChannelUserCompleter::setType(ChannelUserCompleterType newType)
{
    mType = newType;
}

QIcon ChannelUserCompleter::statusIcon() const
{
    return mStatusIcon;
}

QString ChannelUserCompleter::userName() const
{
    return mUserName;
}

void ChannelUserCompleter::setUserName(const QString &newUserName)
{
    mUserName = newUserName;
}

QString ChannelUserCompleter::avatarTag() const
{
    return mAvatarTag;
}

void ChannelUserCompleter::setAvatarTag(const QString &newAvatarTag)
{
    mAvatarTag = newAvatarTag;
}

bool ChannelUserCompleter::outsideRoom() const
{
    return mOutsideRoom;
}

void ChannelUserCompleter::setOutsideRoom(bool newOutsideRoom)
{
    mOutsideRoom = newOutsideRoom;
}

#include "moc_channelusercompleter.cpp"
