/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsmuteuserjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
RoomsMuteUserJob::RoomsMuteUserJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomsMuteUserJob::~RoomsMuteUserJob() = default;

bool RoomsMuteUserJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RoomsMuteUserJob::start");
    submitPostRequest(json());

    return true;
}

void RoomsMuteUserJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("RoomsMuteUserJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT cleanHistoryDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("RoomsMuteUserJob: problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

RoomsMuteUserJob::CleanHistoryInfo RoomsMuteUserJob::cleanHistoryInfo() const
{
    return mCleanHistoryInfo;
}

void RoomsMuteUserJob::setCleanHistoryInfo(const CleanHistoryInfo &cleanHistoryInfo)
{
    mCleanHistoryInfo = cleanHistoryInfo;
}

bool RoomsMuteUserJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomsMuteUserJob::canStart() const
{
    if (!mCleanHistoryInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "mCleanHistoryInfo: mCleanHistoryInfo is not valid.";
        return false;
    }

    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument RoomsMuteUserJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["roomId"_L1] = QLatin1StringView(mCleanHistoryInfo.roomId);
    if (mCleanHistoryInfo.inclusive) {
        jsonObj["inclusive"_L1] = true;
    }
    if (mCleanHistoryInfo.ignoreThreads) {
        jsonObj["ignoreThreads"_L1] = true;
    }
    if (mCleanHistoryInfo.filesOnly) {
        jsonObj["filesOnly"_L1] = true;
    }
    if (mCleanHistoryInfo.excludePinned) {
        jsonObj["excludePinned"_L1] = true;
    }
    if (mCleanHistoryInfo.ignoreDiscussion) {
        jsonObj["ignoreDiscussion"_L1] = true;
    }

    jsonObj["latest"_L1] = mCleanHistoryInfo.latest.toString(Qt::ISODateWithMs);
    jsonObj["oldest"_L1] = mCleanHistoryInfo.oldest.toString(Qt::ISODateWithMs);
    if (!mCleanHistoryInfo.users.isEmpty()) {
        jsonObj["users"_L1] = QJsonArray::fromStringList(mCleanHistoryInfo.users);
    }

    const QJsonDocument postData = QJsonDocument(jsonObj);
    // qDebug() << " postData**************** " << postData;
    return postData;
}

QNetworkRequest RoomsMuteUserJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsCleanHistory);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

bool RoomsMuteUserJob::CleanHistoryInfo::isValid() const
{
    return latest.isValid() && oldest.isValid() && !roomId.isEmpty();
}

QDebug operator<<(QDebug d, const RocketChatRestApi::RoomsMuteUserJob::CleanHistoryInfo &t)
{
    d << "latest " << t.latest;
    d << "oldest " << t.oldest;
    d << "roomId " << t.roomId;
    d << "users " << t.users;
    d << "inclusive " << t.inclusive;
    d << "excludePinned " << t.excludePinned;
    d << "filesOnly " << t.filesOnly;
    d << "ignoreThreads " << t.ignoreThreads;
    return d;
}

#include "moc_roomsmuteuserjob.cpp"
