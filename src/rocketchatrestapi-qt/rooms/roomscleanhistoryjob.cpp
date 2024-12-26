/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomscleanhistoryjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
RoomsCleanHistoryJob::RoomsCleanHistoryJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomsCleanHistoryJob::~RoomsCleanHistoryJob() = default;

bool RoomsCleanHistoryJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChannelCleanHistoryJob::start");
    submitPostRequest(json());

    return true;
}

void RoomsCleanHistoryJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ChannelCleanHistoryJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT cleanHistoryDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ChannelCleanHistoryJob: problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

RoomsCleanHistoryJob::CleanHistoryInfo RoomsCleanHistoryJob::cleanHistoryInfo() const
{
    return mCleanHistoryInfo;
}

void RoomsCleanHistoryJob::setCleanHistoryInfo(const CleanHistoryInfo &cleanHistoryInfo)
{
    mCleanHistoryInfo = cleanHistoryInfo;
}

bool RoomsCleanHistoryJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomsCleanHistoryJob::canStart() const
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

QJsonDocument RoomsCleanHistoryJob::json() const
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

QNetworkRequest RoomsCleanHistoryJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsCleanHistory);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

bool RoomsCleanHistoryJob::CleanHistoryInfo::isValid() const
{
    return latest.isValid() && oldest.isValid() && !roomId.isEmpty();
}

QDebug operator<<(QDebug d, const RocketChatRestApi::RoomsCleanHistoryJob::CleanHistoryInfo &t)
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

#include "moc_roomscleanhistoryjob.cpp"
