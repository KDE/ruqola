/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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

SyncThreadMessagesJob::~SyncThreadMessagesJob()
{
}

bool SyncThreadMessagesJob::requireHttpAuthentication() const
{
    return true;
}

bool SyncThreadMessagesJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start SyncThreadMessagesJob";
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
    QNetworkReply *reply = mNetworkAccessManager->get(request());
    connect(reply, &QNetworkReply::finished, this, &SyncThreadMessagesJob::slotSyncThreadMessagesFinished);
    addLoggerInfo(QByteArrayLiteral("SyncThreadMessagesJob: update threads message in room"));
    return true;
}

void SyncThreadMessagesJob::slotSyncThreadMessagesFinished()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("SyncThreadMessagesJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT syncThreadMessagesDone(replyObject, mThreadMessageId);
        } else {
            emitFailedMessage(replyObject);
            addLoggerWarning(QByteArrayLiteral("SyncThreadMessagesJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
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
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    addAuthRawHeader(request);
    return request;
}
