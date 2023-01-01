/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getpersonalaccesstokensjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
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
    addStartRestApiInfo(QByteArrayLiteral("GetPersonalAccessTokensJob: Ask custom sounds info"));

    return true;
}

void GetPersonalAccessTokensJob::onGetRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("GetPersonalAccessTokensJob done: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT getPersonalAccessTokensDone(replyObject);
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("GetPersonalAccessTokensJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
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
