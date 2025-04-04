/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatemessagejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
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

void TranslateMessageJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("TranslateMessageJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT translateMessageDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("TranslateMessageJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
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
    jsonObj["messageId"_L1] = mMessageId;
    jsonObj["targetLanguage"_L1] = mTargetLanguage;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_translatemessagejob.cpp"
