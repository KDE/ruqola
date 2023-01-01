/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatemessagejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
TranslateMessageJob::TranslateMessageJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

TranslateMessageJob::~TranslateMessageJob() = default;

bool TranslateMessageJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    submitPostRequest(json());
    return true;
}

void TranslateMessageJob::onPostRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("TranslateMessageJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT translateMessageDone();
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("TranslateMessageJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString TranslateMessageJob::targetLanguage() const
{
    return mTargetLanguage;
}

void TranslateMessageJob::setTargetLanguage(const QString &targetLanguage)
{
    mTargetLanguage = targetLanguage;
}

QString TranslateMessageJob::messageId() const
{
    return mMessageId;
}

void TranslateMessageJob::setMessageId(const QString &messageId)
{
    mMessageId = messageId;
}

bool TranslateMessageJob::requireHttpAuthentication() const
{
    return true;
}

bool TranslateMessageJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mMessageId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TranslateMessageJob: mMessageId is empty";
        return false;
    }
    if (mTargetLanguage.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TranslateMessageJob: mTargetLanguage is empty";
        return false;
    }
    return true;
}

QNetworkRequest TranslateMessageJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::AutoTranslateTranslateMessage);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument TranslateMessageJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("messageId")] = mMessageId;
    jsonObj[QLatin1String("targetLanguage")] = mTargetLanguage;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
