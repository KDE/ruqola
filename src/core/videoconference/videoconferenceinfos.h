/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "videoconferenceinfo.h"
#include <QList>

class LIBRUQOLACORE_EXPORT VideoConferenceInfos
{
public:
    VideoConferenceInfos();

    [[nodiscard]] int offset() const;
    void setOffset(int newOffset);

    [[nodiscard]] int total() const;
    void setTotal(int newTotal);

    [[nodiscard]] int videoConferenceInfosCount() const;
    void setVideoConferenceInfosCount(int newVideoConferenceInfosCount);

    [[nodiscard]] const QList<VideoConferenceInfo> &videoConferenceInfosList() const;
    void setVideoConferenceInfosList(const QList<VideoConferenceInfo> &newVideoConferenceInfosList);

    [[nodiscard]] bool isEmpty() const;
    void clear();
    [[nodiscard]] int count() const;
    [[nodiscard]] VideoConferenceInfo at(int index) const;

    void parseVideoConferenceInfos(const QJsonObject &videoConferenceInfosObj);
    void parseMoreVideoConferenceInfos(const QJsonObject &videoConferenceInfosObj);

    VideoConferenceInfo takeAt(int index);

private:
    LIBRUQOLACORE_NO_EXPORT void parseVideoConferenceInfosObj(const QJsonObject &videoConferenceInfosObj);
    QList<VideoConferenceInfo> mVideoConferenceInfosList;
    int mVideoConferenceInfosCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};
QT_DECL_METATYPE_EXTERN_TAGGED(VideoConferenceInfos, Ruqola_VideoConferenceInfos, LIBRUQOLACORE_EXPORT)
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const VideoConferenceInfos &t);
