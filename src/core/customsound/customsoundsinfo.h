/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "customsoundinfo.h"
#include "libruqolacore_export.h"
#include "paginatedinfolist.h"
class QDebug;

class LIBRUQOLACORE_EXPORT CustomSoundsInfo : public PaginatedInfoList<CustomSoundInfo, &CustomSoundInfo::parseCustomSoundInfo>
{
public:
    void parseCustomSounds(const QJsonObject &obj);
    void parseMoreCustomSounds(const QJsonObject &obj);
};
QT_DECL_METATYPE_EXTERN_TAGGED(CustomSoundsInfo, Ruqola_CustomSoundsInfo, LIBRUQOLACORE_EXPORT)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const CustomSoundsInfo &t);
