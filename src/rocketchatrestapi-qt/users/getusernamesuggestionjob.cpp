/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getusernamesuggestionjob.h"

#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
GetUsernameSuggestionJob::GetUsernameSuggestionJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetUsernameSuggestionJob::~GetUsernameSuggestionJob() = default;

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
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("GetUsernameSuggestionJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        const QString result = replyObject["result"_L1].toString();
        Q_EMIT getUsernameSuggestionDone(result);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("GetUsernameSuggestionJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
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
