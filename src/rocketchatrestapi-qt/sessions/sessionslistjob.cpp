/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sessionslistjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
SessionsListJob::SessionsListJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SessionsListJob::~SessionsListJob() = default;

bool SessionsListJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start SessionsListJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo("SessionsListJob: Ask custom sounds info"_ba);

    return true;
}

void SessionsListJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("SessionsListJob done: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT sessionsListDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("SessionsListJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool SessionsListJob::requireHttpAuthentication() const
{
    return true;
}

QNetworkRequest SessionsListJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::SessionsList);

    QUrlQuery queryUrl;
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);
    QNetworkRequest req(url);
    addAuthRawHeader(req);
    return req;
}

bool SessionsListJob::hasQueryParameterSupport() const
{
    return true;
}

#include "moc_sessionslistjob.cpp"
