/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamsautocompletejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
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

    addStartRestApiInfo("TeamsAutoCompleteJob: ask list of rooms in team"_ba);
    return true;
}

void TeamsAutoCompleteJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("TeamsAutoCompleteJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT teamAutoCompleteDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("TeamsAutoCompleteJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
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
    queryUrl.addQueryItem(u"name"_s, mName);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

bool TeamsAutoCompleteJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mName.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TeamsAutoCompleteJob: mName is empty";
        return false;
    }
    return true;
}

#include "moc_teamsautocompletejob.cpp"
