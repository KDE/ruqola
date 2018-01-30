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

Emoji::Emoji()
{
}

Emoji::~Emoji()
{
}

void Emoji::parseEmoji(const QJsonObject &emoji)
{
    mIdentifier = emoji.value(QLatin1String("_id")).toString();
    mExtension = emoji.value(QLatin1String("extension")).toString();
    mName = emoji.value(QLatin1String("name")).toString();
    const QJsonArray array = emoji.value(QLatin1String("aliases")).toArray();
    const int arrayCount = array.count();
    QStringList lst;
    lst.reserve(arrayCount);
    for (int i = 0; i < arrayCount; ++i) {
        lst.append(array.at(i).toString());
    }
    mAliases = lst;
    //TODO updated ?
}

bool Emoji::isValid() const
{
    //Add more check ?
    return !mIdentifier.isEmpty() && !mName.isEmpty();
}

QString Emoji::html() const
{
    //TODO generate url
    return {};
}

QStringList Emoji::aliases() const
{
    return mAliases;
}

void Emoji::setAliases(const QStringList &aliases)
{
    mAliases = aliases;
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
             (mAliases == other.aliases()));
}

Emoji &Emoji::operator=(const Emoji &other)
{
    mName = other.name();
    mExtension = other.extension();
    mIdentifier = other.identifier();
    mAliases = other.aliases();
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
