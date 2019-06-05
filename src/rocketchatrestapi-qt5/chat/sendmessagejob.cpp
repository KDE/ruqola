/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

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

#include "sendmessagejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
SendMessageJob::SendMessageJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SendMessageJob::~SendMessageJob()
{
}

bool SendMessageJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);

    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    addLoggerInfo("SendMessageJob started " + baPostData);
    connect(reply, &QNetworkReply::finished, this, &SendMessageJob::slotSendMessageDone);
    return true;
}

bool SendMessageJob::requireHttpAuthentication() const
{
    return true;
}

void SendMessageJob::slotSendMessageDone()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("SendMessageJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT sendMessageDone();
        } else {
            emitFailedMessage(replyObject);
            addLoggerWarning(QByteArrayLiteral("SendMessageJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

SendMessageJob::SendMessageArguments SendMessageJob::sendMessageArguments() const
{
    return mSendMessageArguments;
}

void SendMessageJob::setSendMessageArguments(const SendMessageArguments &sendMessageArguments)
{
    mSendMessageArguments = sendMessageArguments;
}

QNetworkRequest SendMessageJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatSendMessage);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

bool SendMessageJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start SendMessageJob job";
        return false;
    }
    if (mSendMessageArguments.message.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Text is empty";
        return false;
    }
    if (mSendMessageArguments.roomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "roomId is not defined";
        return false;
    }
    return true;
}

QJsonDocument SendMessageJob::json() const
{
    QJsonObject message;
    QJsonObject jsonObj;
    if (!mSendMessageArguments.messageId.isEmpty()) {
        jsonObj[QLatin1String("_id")] = mSendMessageArguments.messageId;
    }
    if (!mSendMessageArguments.threadMessageId.isEmpty()) {
        jsonObj[QLatin1String("tmid")] = mSendMessageArguments.threadMessageId;
    }
    jsonObj[QLatin1String("rid")] = mSendMessageArguments.roomId;
    jsonObj[QLatin1String("msg")] = mSendMessageArguments.message;
    message[QLatin1String("message")] = jsonObj;
    const QJsonDocument postData = QJsonDocument(message);
    return postData;
}
