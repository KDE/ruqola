/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "statisticsjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
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

    addStartRestApiInfo(QByteArrayLiteral("StatisticsJob: Ask for server statistics"));
    return true;
}

void StatisticsJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("StatisticsJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT statisticDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("StatisticsJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
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
    queryUrl.addQueryItem(QStringLiteral("refresh"), mRefresh ? QStringLiteral("true") : QStringLiteral("false"));
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

#include "moc_statisticsjob.cpp"
