/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reactonmessagejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

using namespace RocketChatRestApi;
ReactOnMessageJob::ReactOnMessageJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ReactOnMessageJob::~ReactOnMessageJob() = default;

bool ReactOnMessageJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ReactOnMessageJob::start");
    submitPostRequest(json());

    return true;
}

void ReactOnMessageJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ReactOnMessageJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT reactOnMessageDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ReactOnMessageJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString ReactOnMessageJob::emoji() const
{
    return mEmoji;
}

void ReactOnMessageJob::setEmoji(const QString &emoji)
{
    mEmoji = emoji;
}

void ReactOnMessageJob::setShouldReact(bool added)
{
    mAddReact = added;
}

bool ReactOnMessageJob::shouldReact() const
{
    return mAddReact;
}

bool ReactOnMessageJob::requireHttpAuthentication() const
{
    return true;
}

bool ReactOnMessageJob::canStart() const
{
    if (mEmoji.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ReactOnMessageJob: emoji is empty";
        return false;
    }
    if (mMessageId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ReactOnMessageJob: messageId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ReactOnMessageJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("emoji")] = mEmoji;
    jsonObj[QLatin1String("messageId")] = mMessageId;
    jsonObj[QLatin1String("shouldReact")] = mAddReact;

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
    addRequestAttribute(request);
    return request;
}

QString RocketChatRestApi::ReactOnMessageJob::errorMessage(const QString &str, const QJsonObject &details)
{
    if (str == QLatin1String("error-not-allowed")) {
        return i18n("Invalid emoji provided.");
    }
    return RestApiAbstractJob::errorMessage(str, details);
}

#include "moc_reactonmessagejob.cpp"
