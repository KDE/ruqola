/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "syncmessagesjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
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

    addStartRestApiInfo("SyncMessagesJob: sync messages in room"_ba);
    return true;
}

void SyncMessagesJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("SyncMessagesJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT syncMessagesDone(replyObject, mRoomId);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("SyncMessagesJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
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
    queryUrl.addQueryItem(u"roomId"_s, QLatin1StringView(mRoomId));
    queryUrl.addQueryItem(u"lastUpdate"_s, mLastUpdate.toUTC().toString(Qt::ISODateWithMs));

    // qDebug() << " queryUrl " << queryUrl.toString();
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addRequestAttribute(request);

    addAuthRawHeader(request);
    return request;
}

bool SyncMessagesJob::hasQueryParameterSupport() const
{
    return true;
}

#include "moc_syncmessagesjob.cpp"
