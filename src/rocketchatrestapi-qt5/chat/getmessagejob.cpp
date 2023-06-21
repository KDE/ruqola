/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getmessagejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
GetMessageJob::GetMessageJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetMessageJob::~GetMessageJob() = default;

bool GetMessageJob::requireHttpAuthentication() const
{
    return true;
}

bool GetMessageJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start get message job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("GetMessageJob: get message starting"));
    return true;
}

void GetMessageJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("GetMessageJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT getMessageDone(replyObject, mMessageId, mRoomId);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("GetMessageJob: Problem when we tried to get message : ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString GetMessageJob::roomId() const
{
    return mRoomId;
}

void GetMessageJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QString GetMessageJob::messageId() const
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
    addRequestAttribute(request, false);

    return request;
}

bool GetMessageJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mMessageId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "MessageId is empty";
        return false;
    }
    return true;
}

#include "moc_getmessagejob.cpp"
