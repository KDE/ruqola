/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customemojisinfo.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(CustomEmojisInfo, Ruqola_CustomEmojisInfo)

using namespace Qt::Literals::StringLiterals;

void CustomEmojisInfo::parseCustomEmojis(const QJsonObject &obj)
{
    parseInfos(obj, "emojis"_L1);
}

void CustomEmojisInfo::parseMoreCustomEmojis(const QJsonObject &obj)
{
    parseMoreInfos(obj, "emojis"_L1);
}

QDebug operator<<(QDebug d, const CustomEmojisInfo &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "loadedCount" << t.loadedCount() << "\n";
    for (const CustomEmoji &emoji : t.list()) {
        d.space() << emoji << "\n";
    }
    return d;
}
