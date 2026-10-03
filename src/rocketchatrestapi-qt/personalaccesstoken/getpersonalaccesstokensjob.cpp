/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getpersonalaccesstokensjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
GetPersonalAccessTokensJob::GetPersonalAccessTokensJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetPersonalAccessTokensJob::~GetPersonalAccessTokensJob() = default;

bool GetPersonalAccessTokensJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start GetPersonalAccessTokensJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo("GetPersonalAccessTokensJob: Ask personal access tokens"_ba);

    return true;
}

void GetPersonalAccessTokensJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (const auto replyObject = checkResponse("GetPersonalAccessTokensJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT getPersonalAccessTokensDone(*replyObject);
    }
}

bool GetPersonalAccessTokensJob::requireHttpAuthentication() const
{
    return true;
}

QNetworkRequest GetPersonalAccessTokensJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersGetPersonalAccessTokens);

    QUrlQuery queryUrl;
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);
    QNetworkRequest req(url);
    addAuthRawHeader(req);
    return req;
}

bool GetPersonalAccessTokensJob::hasQueryParameterSupport() const
{
    return false;
}

#include "moc_getpersonalaccesstokensjob.cpp"
