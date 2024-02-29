/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferenceinfos.h"
#include "ruqola_debug.h"

#include <QJsonArray>
#include <QJsonObject>

VideoConferenceInfos::VideoConferenceInfos() = default;

QDebug operator<<(QDebug d, const VideoConferenceInfos &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "VideoConferenceInfosCount" << t.videoconferenceInfosCount() << "\n";
    for (int i = 0, total = t.videoconferenceInfosList().count(); i < total; ++i) {
        d.space() << t.videoconferenceInfosList().at(i) << "\n";
    }
    return d;
}

int VideoConferenceInfos::offset() const
{
    return mOffset;
}

void VideoConferenceInfos::setOffset(int newOffset)
{
    mOffset = newOffset;
}

int VideoConferenceInfos::total() const
{
    return mTotal;
}

void VideoConferenceInfos::setTotal(int newTotal)
{
    mTotal = newTotal;
}

int VideoConferenceInfos::videoconferenceInfosCount() const
{
    return mVideoConferenceInfosCount;
}

void VideoConferenceInfos::setVideoConferenceInfosCount(int newVideoConferenceInfosCount)
{
    mVideoConferenceInfosCount = newVideoConferenceInfosCount;
}

const QList<VideoConferenceInfo> &VideoConferenceInfos::videoconferenceInfosList() const
{
    return mVideoConferenceInfosList;
}

void VideoConferenceInfos::setVideoConferenceInfosList(const QList<VideoConferenceInfo> &newVideoConferenceInfosList)
{
    mVideoConferenceInfosList = newVideoConferenceInfosList;
}

bool VideoConferenceInfos::isEmpty() const
{
    return mVideoConferenceInfosList.isEmpty();
}

void VideoConferenceInfos::clear()
{
    mVideoConferenceInfosList.clear();
}

int VideoConferenceInfos::count() const
{
    return mVideoConferenceInfosList.count();
}

VideoConferenceInfo VideoConferenceInfos::at(int index) const
{
    if (index < 0 || index > mVideoConferenceInfosList.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        return {};
    }
    return mVideoConferenceInfosList.at(index);
}

void VideoConferenceInfos::parseVideoConferenceInfos(const QJsonObject &VideoConferenceInfosObj)
{
    mVideoConferenceInfosList.clear();
    mVideoConferenceInfosCount = VideoConferenceInfosObj[QLatin1String("count")].toInt();
    mOffset = VideoConferenceInfosObj[QLatin1String("offset")].toInt();
    mTotal = VideoConferenceInfosObj[QLatin1String("total")].toInt();
    mVideoConferenceInfosList.reserve(mVideoConferenceInfosCount);
    parseVideoConferenceInfosObj(VideoConferenceInfosObj);
}

void VideoConferenceInfos::parseVideoConferenceInfosObj(const QJsonObject &VideoConferenceInfosObj)
{
    const QJsonArray discussionsArray = VideoConferenceInfosObj[QLatin1String("sessions")].toArray();
    for (const QJsonValue &current : discussionsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject discussionObject = current.toObject();
            VideoConferenceInfo m;
            m.parse(discussionObject);
            mVideoConferenceInfosList.append(std::move(m));
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing discussions" << current;
        }
    }
}

void VideoConferenceInfos::parseMoreVideoConferenceInfos(const QJsonObject &VideoConferenceInfosObj)
{
    const int VideoConferenceInfosCount = VideoConferenceInfosObj[QLatin1String("count")].toInt();
    mOffset = VideoConferenceInfosObj[QLatin1String("offset")].toInt();
    mTotal = VideoConferenceInfosObj[QLatin1String("total")].toInt();
    parseVideoConferenceInfosObj(VideoConferenceInfosObj);
    mVideoConferenceInfosCount += VideoConferenceInfosCount;
}

VideoConferenceInfo VideoConferenceInfos::takeAt(int index)
{
    return mVideoConferenceInfosList.takeAt(index);
}
