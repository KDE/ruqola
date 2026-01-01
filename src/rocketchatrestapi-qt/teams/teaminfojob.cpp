/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teaminfojob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>
using namespace RocketChatRestApi;
TeamInfoJob::TeamInfoJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

TeamInfoJob::~TeamInfoJob() = default;

bool TeamInfoJob::requireHttpAuthentication() const
{
    return true;
}

bool TeamInfoJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start TeamInfoJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("TeamInfoJob: ask list of rooms in team"_ba);
    return true;
}

void TeamInfoJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("TeamInfoJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT teamInfoDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("TeamInfoJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString TeamInfoJob::teamId() const
{
    return mTeamId;
}

void TeamInfoJob::setTeamId(const QString &teamId)
{
    mTeamId = teamId;
}

QNetworkRequest TeamInfoJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::TeamsInfo);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(u"teamId"_s, mTeamId);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

bool TeamInfoJob::canStart() const
{
    if (mTeamId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TeamInfoJob: mTeamId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

#include "moc_teaminfojob.cpp"
