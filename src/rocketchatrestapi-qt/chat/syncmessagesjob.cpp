/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "syncmessagesjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
using namespace RocketChatRestApi;
SyncMessagesJob::SyncMessagesJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SyncMessagesJob::~SyncMessagesJob() = default;

bool SyncMessagesJob::requireHttpAuthentication() const
{
    return true;
}

bool SyncMessagesJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SyncMessagesJob: mRoomId is empty";
        return false;
    }
    if (!mLastUpdate.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SyncMessagesJob: mLastUpdate is invalid";
        return false;
    }
    return true;
}

bool SyncMessagesJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start SyncMessagesJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("SyncMessagesJob: sync messages in room"));
    return true;
}

void SyncMessagesJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QLatin1StringView("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("SyncMessagesJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT syncMessagesDone(replyObject, mRoomId);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("SyncMessagesJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QDateTime SyncMessagesJob::lastUpdate() const
{
    return mLastUpdate;
}

void SyncMessagesJob::setLastUpdate(const QDateTime &newLastUpdate)
{
    mLastUpdate = newLastUpdate;
}

QByteArray SyncMessagesJob::roomId() const
{
    return mRoomId;
}

void SyncMessagesJob::setRoomId(const QByteArray &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest SyncMessagesJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatSyncMessages);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("roomId"), QLatin1StringView(mRoomId));
    queryUrl.addQueryItem(QStringLiteral("lastUpdate"), mLastUpdate.toUTC().toString(Qt::ISODateWithMs));

    // qDebug() << " queryUrl " << queryUrl.toString();
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addRequestAttribute(request, false);

    addAuthRawHeader(request);
    return request;
}

bool SyncMessagesJob::hasQueryParameterSupport() const
{
    return true;
}

#include "moc_syncmessagesjob.cpp"
