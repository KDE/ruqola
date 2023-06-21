/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomscleanhistoryjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
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

    if (replyObject[QStringLiteral("success")].toBool()) {
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
    jsonObj[QLatin1String("roomId")] = mCleanHistoryInfo.roomId;
    if (mCleanHistoryInfo.inclusive) {
        jsonObj[QLatin1String("inclusive")] = true;
    }
    if (mCleanHistoryInfo.ignoreThreads) {
        jsonObj[QLatin1String("ignoreThreads")] = true;
    }
    if (mCleanHistoryInfo.filesOnly) {
        jsonObj[QLatin1String("filesOnly")] = true;
    }
    if (mCleanHistoryInfo.excludePinned) {
        jsonObj[QLatin1String("excludePinned")] = true;
    }
    if (mCleanHistoryInfo.ignoreDiscussion) {
        jsonObj[QLatin1String("ignoreDiscussion")] = true;
    }

    jsonObj[QLatin1String("latest")] = mCleanHistoryInfo.latest.toString(Qt::ISODateWithMs);
    jsonObj[QLatin1String("oldest")] = mCleanHistoryInfo.oldest.toString(Qt::ISODateWithMs);
    if (!mCleanHistoryInfo.users.isEmpty()) {
        jsonObj[QLatin1String("users")] = QJsonArray::fromStringList(mCleanHistoryInfo.users);
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
