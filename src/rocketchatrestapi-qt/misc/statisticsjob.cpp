/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "statisticsjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
using namespace RocketChatRestApi;
StatisticsJob::StatisticsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

StatisticsJob::~StatisticsJob() = default;

bool StatisticsJob::requireHttpAuthentication() const
{
    return true;
}

bool StatisticsJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start StatisticsJob";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("StatisticsJob: Ask for server statistics"_ba);
    return true;
}

void StatisticsJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("StatisticsJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT statisticDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("StatisticsJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool StatisticsJob::refresh() const
{
    return mRefresh;
}

void StatisticsJob::setRefresh(bool refresh)
{
    mRefresh = refresh;
}

QNetworkRequest StatisticsJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Statistics);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(u"refresh"_s, mRefresh ? u"true"_s : u"false"_s);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

#include "moc_statisticsjob.cpp"
