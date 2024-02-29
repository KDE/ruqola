/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "videoconferenceinfo.h"
#include <QDebug>
#include <QList>
#include <QString>

class LIBRUQOLACORE_EXPORT VideoConferenceInfos
{
public:
    VideoConferenceInfos();

    [[nodiscard]] int offset() const;
    void setOffset(int newOffset);

    [[nodiscard]] int total() const;
    void setTotal(int newTotal);

    [[nodiscard]] int videoconferenceInfosCount() const;
    void setVideoConferenceInfosCount(int newVideoConferenceInfosCount);

    [[nodiscard]] const QList<VideoConferenceInfo> &videoconferenceInfosList() const;
    void setVideoConferenceInfosList(const QList<VideoConferenceInfo> &newVideoConferenceInfosList);

    [[nodiscard]] bool isEmpty() const;
    void clear();
    [[nodiscard]] int count() const;
    [[nodiscard]] VideoConferenceInfo at(int index) const;

    void parseVideoConferenceInfos(const QJsonObject &discussionsObj);
    void parseMoreVideoConferenceInfos(const QJsonObject &videoconferenceInfosObj);

    VideoConferenceInfo takeAt(int index);

private:
    LIBRUQOLACORE_NO_EXPORT void parseVideoConferenceInfosObj(const QJsonObject &discussionsObj);
    QList<VideoConferenceInfo> mVideoConferenceInfosList;
    int mVideoConferenceInfosCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};
Q_DECLARE_METATYPE(VideoConferenceInfos)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const VideoConferenceInfos &t);
