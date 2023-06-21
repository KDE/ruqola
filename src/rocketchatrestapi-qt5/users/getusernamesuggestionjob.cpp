/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getusernamesuggestionjob.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
GetUsernameSuggestionJob::GetUsernameSuggestionJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetUsernameSuggestionJob::~GetUsernameSuggestionJob() = default;

bool GetUsernameSuggestionJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

bool GetUsernameSuggestionJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo("GetUsernameSuggestionJob start");
    return true;
}

void GetUsernameSuggestionJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("GetUsernameSuggestionJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        const QString result = replyObject[QStringLiteral("result")].toString();
        Q_EMIT getUsernameSuggestionDone(result);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("GetUsernameSuggestionJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QNetworkRequest GetUsernameSuggestionJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersGetUsernameSuggestion);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    return request;
}

bool GetUsernameSuggestionJob::requireHttpAuthentication() const
{
    return true;
}

#include "moc_getusernamesuggestionjob.cpp"
