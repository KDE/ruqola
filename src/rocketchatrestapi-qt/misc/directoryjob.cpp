/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directoryjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
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

    addStartRestApiInfo("DirectoryJob: Ask for search room or users or teams"_ba);
    return true;
}

void DirectoryJob::initialQueryParameters()
{
    QueryParameters parameters = queryParameters();
    QMap<QString, QString> customMap;
    switch (mDirectoryInfo.searchType) {
    case SearchType::Unknown:
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "mDirectoryInfo.searchType is undefined";
        break;
    case SearchType::Rooms:
        customMap.insert(QStringLiteral("type"), QStringLiteral("channels"));
        break;
    case SearchType::Users:
        customMap.insert(QStringLiteral("type"), QStringLiteral("users"));
        break;
    case SearchType::Teams:
        customMap.insert(QStringLiteral("type"), QStringLiteral("teams")); // Verify
        break;
    }

    customMap.insert(QStringLiteral("text"), mDirectoryInfo.pattern);
    customMap.insert(QStringLiteral("workspace"), QStringLiteral("local"));
    parameters.setCustom(customMap);
    setQueryParameters(parameters);
}

void DirectoryJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("DirectoryJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT directoryDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("DirectoryJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
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
    addRequestAttribute(request);
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
