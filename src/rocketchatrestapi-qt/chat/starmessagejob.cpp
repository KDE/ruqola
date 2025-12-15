/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "starmessagejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
StarMessageJob::StarMessageJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

StarMessageJob::~StarMessageJob() = default;

bool StarMessageJob::canStart() const
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

bool StarMessageJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("StarMessageJob::start");
    submitPostRequest(json());

    return true;
}

void StarMessageJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("StarMessageJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT messageStarred();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("StarMessageJob: problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool StarMessageJob::starMessage() const
{
    return mStarMessage;
}

void StarMessageJob::setStarMessage(bool starMessage)
{
    mStarMessage = starMessage;
}

QJsonDocument StarMessageJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["messageId"_L1] = QLatin1StringView(mMessageId);

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool StarMessageJob::requireHttpAuthentication() const
{
    return true;
}

QByteArray StarMessageJob::messageId() const
{
    return mMessageId;
}

void StarMessageJob::setMessageId(const QByteArray &messageId)
{
    mMessageId = messageId;
}

QNetworkRequest StarMessageJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(mStarMessage ? RestApiUtil::RestApiUrlType::ChatStarMessage : RestApiUtil::RestApiUrlType::ChatUnStarMessage);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_starmessagejob.cpp"
