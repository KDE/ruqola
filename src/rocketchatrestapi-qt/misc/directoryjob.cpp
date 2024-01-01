/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directoryjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
DirectoryJob::DirectoryJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

DirectoryJob::~DirectoryJob() = default;

bool DirectoryJob::requireHttpAuthentication() const
{
    return true;
}

bool DirectoryJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    initialQueryParameters();
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("DirectoryJob: Ask for search room or users or teams"));
    return true;
}

void DirectoryJob::initialQueryParameters()
{
    QueryParameters parameters = queryParameters();
    QMap<QString, QString> map;
    switch (mDirectoryInfo.searchType) {
    case Unknown:
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "mDirectoryInfo.searchType is undefined";
        break;
    case Rooms:
        map.insert(QStringLiteral("type"), QStringLiteral("channels"));
        break;
    case Users:
        map.insert(QStringLiteral("type"), QStringLiteral("users"));
        break;
    case Teams:
        map.insert(QStringLiteral("type"), QStringLiteral("teams")); // Verify
        break;
    }

    map.insert(QStringLiteral("text"), mDirectoryInfo.pattern);
    map.insert(QStringLiteral("workspace"), QStringLiteral("local"));
    parameters.setCustom(map);
    setQueryParameters(parameters);
}

void DirectoryJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("DirectoryJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT directoryDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("DirectoryJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

DirectoryJob::DirectoryInfo DirectoryJob::directoryInfo() const
{
    return mDirectoryInfo;
}

void DirectoryJob::setDirectoryInfo(const DirectoryInfo &directoryInfo)
{
    mDirectoryInfo = directoryInfo;
}

bool DirectoryJob::hasQueryParameterSupport() const
{
    return true;
}

QNetworkRequest DirectoryJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Directory);
    QUrlQuery queryUrl;
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);
    return request;
}

bool DirectoryJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mDirectoryInfo.canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "DirectoryJob: mSearchType is undefined";
        return false;
    }
    return true;
}

bool DirectoryJob::DirectoryInfo::canStart() const
{
    return searchType != SearchType::Unknown;
}

bool DirectoryJob::DirectoryInfo::operator==(const DirectoryJob::DirectoryInfo &other) const
{
    return other.searchType == searchType && other.pattern == pattern;
}

#include "moc_directoryjob.cpp"
