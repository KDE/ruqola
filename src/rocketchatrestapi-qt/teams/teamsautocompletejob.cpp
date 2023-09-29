/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamsautocompletejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
TeamsAutoCompleteJob::TeamsAutoCompleteJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

TeamsAutoCompleteJob::~TeamsAutoCompleteJob() = default;

bool TeamsAutoCompleteJob::requireHttpAuthentication() const
{
    return true;
}

bool TeamsAutoCompleteJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start TeamsAutoCompleteJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("TeamsAutoCompleteJob: ask list of rooms in team"));
    return true;
}

void TeamsAutoCompleteJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("TeamsAutoCompleteJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT teamAutoCompleteDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("TeamsAutoCompleteJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

const QString &TeamsAutoCompleteJob::name() const
{
    return mName;
}

void TeamsAutoCompleteJob::setName(const QString &newName)
{
    mName = newName;
}

QNetworkRequest TeamsAutoCompleteJob::request() const
{
    // api/v1/teams.autocomplete?name=f
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::TeamsAutocomplete);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("name"), mName);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

bool TeamsAutoCompleteJob::canStart() const
{
    if (mName.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TeamsAutoCompleteJob: mName is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

#include "moc_teamsautocompletejob.cpp"
