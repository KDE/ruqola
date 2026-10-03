/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customsoundsinfo.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(CustomSoundsInfo, Ruqola_CustomSoundsInfo)

using namespace Qt::Literals::StringLiterals;

void CustomSoundsInfo::parseCustomSounds(const QJsonObject &obj)
{
    parseInfos(obj, "sounds"_L1);
}

void CustomSoundsInfo::parseMoreCustomSounds(const QJsonObject &obj)
{
    parseMoreInfos(obj, "sounds"_L1);
}

QDebug operator<<(QDebug d, const CustomSoundsInfo &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "loadedCount" << t.loadedCount() << "\n";
    for (const CustomSoundInfo &info : t.list()) {
        d.space() << info << "\n";
    }
    return d;
}
