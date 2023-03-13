/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "spotlightjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
SpotlightJob::SpotlightJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SpotlightJob::~SpotlightJob() = default;

bool SpotlightJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start SpotlightJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo(QByteArrayLiteral("SpotlightJob start"));
    return true;
}

void SpotlightJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("SpotlightJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT spotlightDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("SpotlightJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString SpotlightJob::searchPattern() const
{
    return mSearchPattern;
}

void SpotlightJob::setSearchPattern(const QString &searchPattern)
{
    mSearchPattern = searchPattern;
}

bool SpotlightJob::requireHttpAuthentication() const
{
    return true;
}

QNetworkRequest SpotlightJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Spotlight);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("query"), mSearchPattern);
    url.setQuery(queryUrl);
    QNetworkRequest req(url);
    addAuthRawHeader(req);
    return req;
}

bool SpotlightJob::canStart() const
{
    if (mSearchPattern.trimmed().isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SpotlightJob: searchpattern is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}
