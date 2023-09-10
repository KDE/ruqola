/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "syncthreadmessagesjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
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

    addStartRestApiInfo(QByteArrayLiteral("SyncThreadMessagesJob: update threads message in room"));
    return true;
}

void SyncThreadMessagesJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("SyncThreadMessagesJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT syncThreadMessagesDone(replyObject, mThreadMessageId);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("SyncThreadMessagesJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
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
    queryUrl.addQueryItem(QStringLiteral("tmid"), mThreadMessageId);
    queryUrl.addQueryItem(QStringLiteral("updatedSince"), mTimeStamp);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addRequestAttribute(request, false);

    addAuthRawHeader(request);
    return request;
}

#include "moc_syncthreadmessagesjob.cpp"
