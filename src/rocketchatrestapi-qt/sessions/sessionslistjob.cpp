/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sessionslistjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
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
    addStartRestApiInfo(QByteArrayLiteral("SessionsListJob: Ask custom sounds info"));

    return true;
}

void SessionsListJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("SessionsListJob done: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT sessionsListDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("SessionsListJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
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
