/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "emoji.h"

#include <QJsonObject>
#include <QJsonArray>

Emoji::Emoji()
{
}

Emoji::~Emoji()
{
}

bool Emoji::hasEmoji(const QString &identifier) const
{
    return ((mEmojiIdentifier == identifier) || mAliases.contains(identifier));
}

void Emoji::parseEmoji(const QJsonObject &emoji)
{
    mIdentifier = emoji.value(QLatin1String("_id")).toString();
    mExtension = emoji.value(QLatin1String("extension")).toString();
    mName = emoji.value(QLatin1String("name")).toString();
    mEmojiIdentifier = QLatin1Char(':') + mName + QLatin1Char(':');
    const QJsonArray array = emoji.value(QLatin1String("aliases")).toArray();
    const int arrayCount = array.count();
    QStringList lst;
    lst.reserve(arrayCount);
    for (int i = 0; i < arrayCount; ++i) {
        lst.append(QLatin1Char(':') + array.at(i).toString() + QLatin1Char(':'));
    }
    mAliases = lst;
    //TODO updated when we will store data on system
}

bool Emoji::isValid() const
{
    //Add more check ?
    return !mIdentifier.isEmpty() && !mName.isEmpty();
}

QString Emoji::html(const QString &serverUrl)
{
    if (mCachedHtml.isEmpty()) {
        //TODO verify it.

        QString url = serverUrl + QStringLiteral("/emoji-custom/%1.%2").arg(mName).arg(mExtension);
        if (!url.startsWith(QStringLiteral("http://"))) {
            url.prepend(QLatin1String("http://"));
        }
        //https://rocket.chat/docs/developer-guides/realtime-api/method-calls/list-custom-emoji/#list-custom-emoji
        //http://yourhost.com/emoji-custom/Emoji%20Name.png
        //TODO customize size.
        mCachedHtml = QStringLiteral("<img height='22' width='22' src='%1'/>").arg(url);
    }
    return mCachedHtml;
}

void Emoji::clearCachedHtml()
{
    mCachedHtml.clear();
}

QStringList Emoji::aliases() const
{
    return mAliases;
}

void Emoji::setAliases(const QStringList &aliases)
{
    mAliases = aliases;
}

QString Emoji::emojiIdentifier() const
{
    return mEmojiIdentifier;
}

void Emoji::setEmojiIdentifier(const QString &emojiIdentifier)
{
    mEmojiIdentifier = emojiIdentifier;
}

QString Emoji::cachedHtml() const
{
    return mCachedHtml;
}

QString Emoji::identifier() const
{
    return mIdentifier;
}

void Emoji::setIdentifier(const QString &identifier)
{
    mIdentifier = identifier;
}

QString Emoji::extension() const
{
    return mExtension;
}

void Emoji::setExtension(const QString &extension)
{
    mExtension = extension;
}

void Emoji::setName(const QString &name)
{
    mName = name;
}

QString Emoji::name() const
{
    return mName;
}

bool Emoji::operator==(const Emoji &other) const
{
    return (mName == other.name()) &&
            (mExtension == other.extension()) &&
            (mIdentifier == other.identifier() &&
             (mAliases == other.aliases()) &&
             (mEmojiIdentifier == other.emojiIdentifier()));
}

Emoji &Emoji::operator=(const Emoji &other)
{
    mName = other.name();
    mExtension = other.extension();
    mIdentifier = other.identifier();
    mAliases = other.aliases();
    mEmojiIdentifier = other.emojiIdentifier();
    return *this;
}

QDebug operator <<(QDebug d, const Emoji &t)
{
    d << "Name: " << t.name();
    d << "Identifier: " << t.identifier();
    d << "extension: " << t.extension();
    d << "aliases: " << t.aliases();
    return d;
}
