/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferenceinfo.h"

VideoConferenceInfo::VideoConferenceInfo() = default;

VideoConferenceInfo::~VideoConferenceInfo() = default;

void VideoConferenceInfo::parse(const QJsonObject &content)
{
    // TODO
}

QString VideoConferenceInfo::url() const
{
    return mUrl;
}

void VideoConferenceInfo::setUrl(const QString &newUrl)
{
    mUrl = newUrl;
}

QDebug operator<<(QDebug d, const VideoConferenceInfo &t)
{
    d << "mUrl " << t.url();
    return d;
}
