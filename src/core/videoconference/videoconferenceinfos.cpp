/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferenceinfos.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(VideoConferenceInfos, Ruqola_VideoConferenceInfos)

using namespace Qt::Literals::StringLiterals;

void VideoConferenceInfos::parseVideoConferenceInfos(const QJsonObject &obj)
{
    parseInfos(obj, "data"_L1);
}

void VideoConferenceInfos::parseMoreVideoConferenceInfos(const QJsonObject &obj)
{
    parseMoreInfos(obj, "data"_L1);
}

QDebug operator<<(QDebug d, const VideoConferenceInfos &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "VideoConferenceInfosCount" << t.loadedCount() << "\n";
    for (const VideoConferenceInfo &info : t.list()) {
        d.space() << info << "\n";
    }
    return d;
}
