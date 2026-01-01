/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customsoundinfo.h"
#include <QJsonObject>
QT_IMPL_METATYPE_EXTERN_TAGGED(CustomSoundInfo, Ruqola_CustomSoundInfo)

using namespace Qt::Literals::StringLiterals;
CustomSoundInfo::CustomSoundInfo() = default;

const QByteArray &CustomSoundInfo::identifier() const
{
    return mIdentifier;
}

void CustomSoundInfo::setIdentifier(const QByteArray &newIdentifier)
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
    mName = obj["name"_L1].toString();
    mIdentifier = obj["_id"_L1].toString().toLatin1();
    mExtension = obj["extension"_L1].toString();
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

bool CustomSoundInfo::defaultSound() const
{
    return mDefaultSound;
}

void CustomSoundInfo::setDefaultSound(bool newDefaultSound)
{
    mDefaultSound = newDefaultSound;
}

QString CustomSoundInfo::generateUrl() const
{
    if (mDefaultSound) {
        return QLatin1StringView("/sounds/%1.%2").arg(QString::fromLatin1(mIdentifier), mExtension);
    } else {
        return QLatin1StringView("/custom-sounds/%1.%2").arg(QString::fromLatin1(mIdentifier), mExtension);
    }
}

QDebug operator<<(QDebug d, const CustomSoundInfo &t)
{
    d.space() << "Identifier:" << t.identifier();
    d.space() << "Name:" << t.name();
    d.space() << "Extension:" << t.extension();
    d.space() << "DefaultSound" << t.defaultSound();
    return d;
}
