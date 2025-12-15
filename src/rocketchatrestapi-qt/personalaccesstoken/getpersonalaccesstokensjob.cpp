/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

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
    addStartRestApiInfo("GetPersonalAccessTokensJob: Ask custom sounds info"_ba);

    return true;
}

void GetPersonalAccessTokensJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("GetPersonalAccessTokensJob done: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT getPersonalAccessTokensDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("GetPersonalAccessTokensJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
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
