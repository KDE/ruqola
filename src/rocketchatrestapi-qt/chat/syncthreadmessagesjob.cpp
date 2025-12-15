/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "syncthreadmessagesjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
SyncThreadMessagesJob::SyncThreadMessagesJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SyncThreadMessagesJob::~SyncThreadMessagesJob() = default;

bool SyncThreadMessagesJob::requireHttpAuthentication() const
{
    return true;
}

bool SyncThreadMessagesJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mThreadMessageId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SyncThreadMessagesJob: mThreadMessageId is empty";
        return false;
    }
    if (mTimeStamp.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SyncThreadMessagesJob: mTimeStamp is empty";
        return false;
    }
    return true;
}

bool SyncThreadMessagesJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start SyncThreadMessagesJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("SyncThreadMessagesJob: update threads message in room"_ba);
    return true;
}

void SyncThreadMessagesJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("SyncThreadMessagesJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT syncThreadMessagesDone(replyObject, mThreadMessageId);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("SyncThreadMessagesJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString SyncThreadMessagesJob::timeStamp() const
{
    return mTimeStamp;
}

void SyncThreadMessagesJob::setTimeStamp(const QString &timeStamp)
{
    mTimeStamp = timeStamp;
}

QString SyncThreadMessagesJob::threadMessageId() const
{
    return mThreadMessageId;
}

void SyncThreadMessagesJob::setThreadMessageId(const QString &threadMessageId)
{
    mThreadMessageId = threadMessageId;
}

QNetworkRequest SyncThreadMessagesJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatSyncThreadMessages);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(u"tmid"_s, mThreadMessageId);
    queryUrl.addQueryItem(u"updatedSince"_s, mTimeStamp);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addRequestAttribute(request);

    addAuthRawHeader(request);
    return request;
}

#include "moc_syncthreadmessagesjob.cpp"
