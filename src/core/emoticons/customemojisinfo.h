/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "customemoji.h"
#include "libruqolacore_export.h"
#include "paginatedinfolist.h"
class QDebug;

namespace CustomEmojisInfoUtils
{
// CustomEmoji::parseEmoji() has a second, defaulted argument, which a member pointer can't carry.
// This has to be declared here: it is the base class's template argument below. The body stays in
// the .cpp, as CustomEmoji itself is only exported for the unit tests.
LIBRUQOLACORE_EXPORT void parseCustomEmoji(CustomEmoji &emoji, const QJsonObject &obj);
}

class LIBRUQOLACORE_EXPORT CustomEmojisInfo : public PaginatedInfoList<CustomEmoji, &CustomEmojisInfoUtils::parseCustomEmoji>
{
public:
    void parseCustomEmojis(const QJsonObject &obj);
    void parseMoreCustomEmojis(const QJsonObject &obj);
};
QT_DECL_METATYPE_EXTERN_TAGGED(CustomEmojisInfo, Ruqola_CustomEmojisInfo, LIBRUQOLACORE_EXPORT)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const CustomEmojisInfo &t);
