/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directoryjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

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
        customMap.insert(u"type"_s, u"channels"_s);
        break;
    case SearchType::Users:
        customMap.insert(u"type"_s, u"users"_s);
        break;
    case SearchType::Teams:
        customMap.insert(u"type"_s, u"teams"_s); // Verify
        break;
    }

    customMap.insert(u"text"_s, mDirectoryInfo.pattern);
    customMap.insert(u"workspace"_s, u"local"_s);
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
