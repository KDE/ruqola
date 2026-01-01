/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamslistjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
TeamsListJob::TeamsListJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

TeamsListJob::~TeamsListJob() = default;

bool TeamsListJob::requireHttpAuthentication() const
{
    return true;
}

bool TeamsListJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start TeamsListJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("TeamsListJob: ask users list"_ba);
    return true;
}

void TeamsListJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("TeamsListJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT teamsListDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("TeamsListJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

const QString &TeamsListJob::pattern() const
{
    return mPattern;
}

void TeamsListJob::setPattern(const QString &newPattern)
{
    mPattern = newPattern;
}

void TeamsListJob::initialQueryParameters()
{
    QueryParameters parameters = queryParameters();
    QMap<QString, QString> map;
    map.insert(u"text"_s, mPattern);
    parameters.setCustom(map);
    setQueryParameters(parameters);
}

QNetworkRequest TeamsListJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::TeamsList);
    QUrlQuery queryUrl;
    // TODO addQueryUrl(url);
    // initialQueryParameters();
    addQueryParameter(queryUrl);
    // TODO verify it url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

bool TeamsListJob::hasQueryParameterSupport() const
{
    return true;
}

#include "moc_teamslistjob.cpp"
