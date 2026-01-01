/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferenceinfos.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(VideoConferenceInfos, Ruqola_VideoConferenceInfos)

#include "ruqola_debug.h"

#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
VideoConferenceInfos::VideoConferenceInfos() = default;

QDebug operator<<(QDebug d, const VideoConferenceInfos &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "VideoConferenceInfosCount" << t.videoConferenceInfosCount() << "\n";
    for (int i = 0, total = t.videoConferenceInfosList().count(); i < total; ++i) {
        d.space() << t.videoConferenceInfosList().at(i) << "\n";
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

int VideoConferenceInfos::videoConferenceInfosCount() const
{
    return mVideoConferenceInfosCount;
}

void VideoConferenceInfos::setVideoConferenceInfosCount(int newVideoConferenceInfosCount)
{
    mVideoConferenceInfosCount = newVideoConferenceInfosCount;
}

const QList<VideoConferenceInfo> &VideoConferenceInfos::videoConferenceInfosList() const
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
    if (index < 0 || index >= mVideoConferenceInfosList.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        return {};
    }
    return mVideoConferenceInfosList.at(index);
}

void VideoConferenceInfos::parseVideoConferenceInfos(const QJsonObject &videoConferenceInfosObj)
{
    mVideoConferenceInfosList.clear();
    mVideoConferenceInfosCount = videoConferenceInfosObj["count"_L1].toInt();
    mOffset = videoConferenceInfosObj["offset"_L1].toInt();
    mTotal = videoConferenceInfosObj["total"_L1].toInt();
    mVideoConferenceInfosList.reserve(mVideoConferenceInfosCount);
    parseVideoConferenceInfosObj(videoConferenceInfosObj);
}

void VideoConferenceInfos::parseVideoConferenceInfosObj(const QJsonObject &videoConferenceInfosObj)
{
    const QJsonArray videoConferencesArray = videoConferenceInfosObj["data"_L1].toArray();
    for (const QJsonValue &current : videoConferencesArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject videoConferenceObject = current.toObject();
            VideoConferenceInfo m;
            m.parse(videoConferenceObject);
            mVideoConferenceInfosList.append(std::move(m));
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing video conference infos" << current;
        }
    }
}

void VideoConferenceInfos::parseMoreVideoConferenceInfos(const QJsonObject &videoConferenceInfosObj)
{
    const int videoConferenceInfosCount = videoConferenceInfosObj["count"_L1].toInt();
    mOffset = videoConferenceInfosObj["offset"_L1].toInt();
    mTotal = videoConferenceInfosObj["total"_L1].toInt();
    parseVideoConferenceInfosObj(videoConferenceInfosObj);
    mVideoConferenceInfosCount += videoConferenceInfosCount;
}

VideoConferenceInfo VideoConferenceInfos::takeAt(int index)
{
    return mVideoConferenceInfosList.takeAt(index);
}
