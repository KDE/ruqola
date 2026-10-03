/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

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
    addStartRestApiInfo("GetUsernameSuggestionJob start"_ba);
    return true;
}

void GetUsernameSuggestionJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (const auto replyObject = checkResponse("GetUsernameSuggestionJob"_ba, replyErrorString, replyJson)) {
        const QString result = replyObject->value("result"_L1).toString();
        Q_EMIT getUsernameSuggestionDone(result);
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
