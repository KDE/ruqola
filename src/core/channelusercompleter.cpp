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
    return d;
}

void ChannelUserCompleter::parseChannel(const QJsonObject &object, ChannelUserCompleterType type)
{
    // qDebug() << " object " << object;
    mType = type;
    mName = object.value(QLatin1String("name")).toString();
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

    // Use "outside"
    // TODO mChannelUserIcon
}

bool ChannelUserCompleter::operator==(const ChannelUserCompleter &other) const
{
    return (mType == other.type()) && (mName == other.name())
        && (mDescription == other.description() && (mUserName == other.userName()) && (mAvatarTag == other.avatarTag()));
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

QIcon ChannelUserCompleter::channelUserIcon() const
{
    return mChannelUserIcon;
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

#include "moc_channelusercompleter.cpp"
