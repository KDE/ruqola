/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#include "getmessagejob.h"
#include "restapimethod.h"
#include "ruqola_restapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>

GetMessageJob::GetMessageJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetMessageJob::~GetMessageJob()
{
}

bool GetMessageJob::requireHttpAuthentication() const
{
    return true;
}

bool GetMessageJob::start()
{
    if (!canStart()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "Impossible to start get message job";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = mNetworkAccessManager->get(request());
    connect(reply, &QNetworkReply::finished, this, &GetMessageJob::slotIgnoreUserFinished);
    addLoggerInfo(QByteArrayLiteral("GetMessageJob: get message starting"));
    return true;
}

void GetMessageJob::slotIgnoreUserFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("GetMessageJob: finished: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT getMessageDone(replyObject);
            qCDebug(RUQOLA_RESTAPI_LOG) << "Ignore user success: " << data;
        } else {
            qCWarning(RUQOLA_RESTAPI_LOG) <<" Problem when we tried to get message";
        }
    }
    deleteLater();
}

QString GetMessageJob::getMessageId() const
{
    return mMessageId;
}

void GetMessageJob::setMessageId(const QString &messageId)
{
    mMessageId = messageId;
}

QNetworkRequest GetMessageJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatGetMessage);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("msgId"), mMessageId);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    return request;
}

bool GetMessageJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "Impossible to start ignore user job";
        return false;
    }
    if (mMessageId.isEmpty()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "MessageId is empty";
        return false;
    }
    return true;
}
