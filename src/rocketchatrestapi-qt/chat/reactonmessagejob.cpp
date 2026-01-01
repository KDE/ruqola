/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reactonmessagejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
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

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("ReactOnMessageJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT reactOnMessageDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("ReactOnMessageJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
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
    jsonObj["emoji"_L1] = mEmoji;
    jsonObj["messageId"_L1] = QLatin1StringView(mMessageId);
    jsonObj["shouldReact"_L1] = mAddReact;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QByteArray ReactOnMessageJob::messageId() const
{
    return mMessageId;
}

void ReactOnMessageJob::setMessageId(const QByteArray &t)
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

QString ReactOnMessageJob::errorMessage(const QString &str, const QJsonObject &details)
{
    if (str == "error-not-allowed"_L1) {
        return i18n("Invalid emoji provided.");
    }
    return RestApiAbstractJob::errorMessage(str, details);
}

#include "moc_reactonmessagejob.cpp"
