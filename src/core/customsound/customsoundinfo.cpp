/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "customsoundinfo.h"

CustomSoundInfo::CustomSoundInfo()
{
}

const QString &CustomSoundInfo::identifier() const
{
    return mIdentifier;
}

void CustomSoundInfo::setIdentifier(const QString &newIdentifier)
{
    mIdentifier = newIdentifier;
}

const QString &CustomSoundInfo::name() const
{
    return mName;
}

void CustomSoundInfo::setName(const QString &newName)
{
    mName = newName;
}

void CustomSoundInfo::parseCustomSoundInfo(const QJsonObject &obj)
{
    mName = obj[QLatin1String("name")].toString();
    mIdentifier = obj[QLatin1String("_id")].toString();
    mExtension = obj[QLatin1String("extension")].toString();
}

const QString &CustomSoundInfo::extension() const
{
    return mExtension;
}

void CustomSoundInfo::setExtension(const QString &newExtension)
{
    mExtension = newExtension;
}

bool CustomSoundInfo::isValid() const
{
    return !mIdentifier.isEmpty() && !mName.isEmpty();
}

QDebug operator<<(QDebug d, const CustomSoundInfo &t)
{
    d << "Identifier: " << t.identifier();
    d << "Name: " << t.name();
    d << "Extension: " << t.extension();
    return d;
}
