/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sendmessagejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
SendMessageJob::SendMessageJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SendMessageJob::~SendMessageJob() = default;

bool SendMessageJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("SendMessageJob::start");
    submitPostRequest(json());

    return true;
}

bool SendMessageJob::requireHttpAuthentication() const
{
    return true;
}

void SendMessageJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("SendMessageJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT sendMessageDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("SendMessageJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
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
        jsonObj["_id"_L1] = mSendMessageArguments.messageId;
    }
    if (!mSendMessageArguments.threadMessageId.isEmpty()) {
        jsonObj["tmid"_L1] = mSendMessageArguments.threadMessageId;
    }
    jsonObj["rid"_L1] = mSendMessageArguments.roomId;
    jsonObj["msg"_L1] = mSendMessageArguments.message;
    message["message"_L1] = jsonObj;
    const QJsonDocument postData = QJsonDocument(message);
    return postData;
}

#include "moc_sendmessagejob.cpp"
