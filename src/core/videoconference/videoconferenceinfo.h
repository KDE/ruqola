/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>

class LIBRUQOLACORE_EXPORT VideoConferenceInfo
{
public:
    VideoConferenceInfo();
    ~VideoConferenceInfo();
    void parse(const QJsonObject &content);

    Q_REQUIRED_RESULT QString url() const;
    void setUrl(const QString &newUrl);

private:
    QString mUrl;
};
Q_DECLARE_METATYPE(VideoConferenceInfo)
Q_DECLARE_TYPEINFO(VideoConferenceInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const VideoConferenceInfo &t);
