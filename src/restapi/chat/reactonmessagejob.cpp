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

#include "reactonmessagejob.h"
#include "ruqola_restapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

ReactOnMessageJob::ReactOnMessageJob(QObject *parent)
    : RestApiAbstractJob(parent)
{

}

ReactOnMessageJob::~ReactOnMessageJob()
{
}

bool ReactOnMessageJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("ReactOnMessageJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &ReactOnMessageJob::slotReactonMessageFinished);
    return true;
}

void ReactOnMessageJob::slotReactonMessageFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            qCDebug(RUQOLA_RESTAPI_LOG) << "React On Message is a success: " << data;
            Q_EMIT reactOnMessageDone();
        } else {
            qCWarning(RUQOLA_RESTAPI_LOG) <<" Problem when we tried to add react on message" << data;
        }
    }
    deleteLater();
}

QString ReactOnMessageJob::emoji() const
{
    return mEmoji;
}

void ReactOnMessageJob::setEmoji(const QString &emoji)
{
    mEmoji = emoji;
}

bool ReactOnMessageJob::requireHttpAuthentication() const
{
    return true;
}

bool ReactOnMessageJob::canStart() const
{
    if (mEmoji.isEmpty()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "ReactOnMessageJob: emoji is empty";
        return false;
    }
    if (mMessageId.isEmpty()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "ReactOnMessageJob: messageId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "Impossible to start ReactOnMessageJob job";
        return false;
    }
    return true;
}

QJsonDocument ReactOnMessageJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("emoji")] = mEmoji;
    jsonObj[QLatin1String("messageId")] = mMessageId;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ReactOnMessageJob::messageId() const
{
    return mMessageId;
}

void ReactOnMessageJob::setMessageId(const QString &t)
{
    mMessageId = t;
}

QNetworkRequest ReactOnMessageJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatReact);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    return request;
}
