/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "updatemessagejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
UpdateMessageJob::UpdateMessageJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UpdateMessageJob::~UpdateMessageJob() = default;

bool UpdateMessageJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("UpdateMessageJob::start");
    submitPostRequest(json());

    return true;
}

void UpdateMessageJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("UpdateMessageJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT updateMessageDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("UpdateMessageJob: problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString UpdateMessageJob::updatedText() const
{
    return mUpdatedText;
}

void UpdateMessageJob::setUpdatedText(const QString &updatedText)
{
    mUpdatedText = updatedText;
}

bool UpdateMessageJob::requireHttpAuthentication() const
{
    return true;
}

bool UpdateMessageJob::canStart() const
{
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "UpdateMessageJob: roomId is empty";
        return false;
    }
    if (mMessageId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "UpdateMessageJob: messageId is empty";
        return false;
    }
    // Updated text I think can be empty.

    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument UpdateMessageJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = mRoomId;
    jsonObj[QLatin1String("msgId")] = mMessageId;
    jsonObj[QLatin1String("text")] = mUpdatedText;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString UpdateMessageJob::roomId() const
{
    return mRoomId;
}

void UpdateMessageJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QString UpdateMessageJob::messageId() const
{
    return mMessageId;
}

void UpdateMessageJob::setMessageId(const QString &t)
{
    mMessageId = t;
}

QNetworkRequest UpdateMessageJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatUpdate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
