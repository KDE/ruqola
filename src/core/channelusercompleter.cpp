/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelusercompleter.h"
#include "utils.h"
#include <QJsonObject>

ChannelUserCompleter::ChannelUserCompleter() = default;

ChannelUserCompleter::~ChannelUserCompleter() = default;

QDebug operator<<(QDebug d, const ChannelUserCompleter &t)
{
    d << "Type " << t.type();
    d << "Name " << t.name();
    d << "description " << t.description();
    d << "userName " << t.userName();
    d << "AvatarTag " << t.avatarTag();
    d << "outsideRoom " << t.outsideRoom();
    d << "identifier " << t.identifier();
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
    mName = object.value(QLatin1String("name")).toString();
    mIdentifier = object.value(QLatin1String("_id")).toString();
    if (mType == ChannelUserCompleterType::DirectChannel) {
        mAvatarTag = object.value(QLatin1String("avatarETag")).toString();
        mUserName = object.value(QLatin1String("username")).toString();
        mStatusIcon = QIcon::fromTheme(Utils::iconFromStatus(object.value(QLatin1String("status")).toString()));
    } else {
        const QString roomType = object.value(QLatin1String("t")).toString();
        if (roomType == QLatin1Char('c')) {
            mStatusIcon = QIcon::fromTheme(QStringLiteral("irc-channel-inactive"));
        } else if (roomType == QLatin1Char('p')) {
            mStatusIcon = QIcon::fromTheme(QStringLiteral("lock"));
        }
    }
    mOutsideRoom = object.value(QLatin1String("outside")).toBool();
    createAvatarInfo();
}

void ChannelUserCompleter::createAvatarInfo()
{
    mAvatarInfo.avatarType = (mType == ChannelUserCompleter::ChannelUserCompleterType::Room ? Utils::AvatarType::Room : Utils::AvatarType::User);
    mAvatarInfo.etag = mAvatarTag;
    mAvatarInfo.identifier = (mType == ChannelUserCompleter::ChannelUserCompleterType::Room ? mIdentifier : mUserName);
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
