/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customsoundinfo.h"
#include <QJsonObject>

CustomSoundInfo::CustomSoundInfo() = default;

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
    d.space() << "Identifier:" << t.identifier();
    d.space() << "Name:" << t.name();
    d.space() << "Extension:" << t.extension();
    return d;
}
