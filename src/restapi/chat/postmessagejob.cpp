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

#include "postmessagejob.h"
#include "restapimethod.h"
#include "ruqola_restapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

PostMessageJob::PostMessageJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

PostMessageJob::~PostMessageJob()
{
}

bool PostMessageJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);

    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    addLoggerInfo("PostMessageJob started ");
    connect(reply, &QNetworkReply::finished, this, &PostMessageJob::slotPostMessageDone);
    return true;
}

bool PostMessageJob::requireHttpAuthentication() const
{
    return true;
}

void PostMessageJob::slotPostMessageDone()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        addLoggerInfo(QByteArrayLiteral("PostMessageJob: finished: ") + data);
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            qCDebug(RUQOLA_RESTAPI_LOG) << "PostMessageJob success";
            Q_EMIT postMessageDone();
        } else {
            qCWarning(RUQOLA_RESTAPI_LOG) <<" Problem when we tried to post message";
        }

    }
    deleteLater();
}

QNetworkRequest PostMessageJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatPostMessage);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    return request;
}

QString PostMessageJob::text() const
{
    return mText;
}

void PostMessageJob::setText(const QString &text)
{
    mText = text;
}

QString PostMessageJob::roomId() const
{
    return mRoomId;
}

void PostMessageJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

bool PostMessageJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "Impossible to start PostMessageJob job";
        return false;
    }
    if (mText.isEmpty()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "Text is empty";
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "roomId is not defined";
        return false;
    }
    return true;
}

QJsonDocument PostMessageJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = mRoomId;
    jsonObj[QLatin1String("text")] = mText;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

