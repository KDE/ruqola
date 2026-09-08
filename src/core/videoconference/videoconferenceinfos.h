/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "paginatedinfolist.h"
#include "videoconferenceinfo.h"
class QDebug;
class LIBRUQOLACORE_EXPORT VideoConferenceInfos : public PaginatedInfoList<VideoConferenceInfo, &VideoConferenceInfo::parse>
{
public:
    void parseVideoConferenceInfos(const QJsonObject &obj);
    void parseMoreVideoConferenceInfos(const QJsonObject &obj);
};
QT_DECL_METATYPE_EXTERN_TAGGED(VideoConferenceInfos, Ruqola_VideoConferenceInfos, LIBRUQOLACORE_EXPORT)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const VideoConferenceInfos &t);
