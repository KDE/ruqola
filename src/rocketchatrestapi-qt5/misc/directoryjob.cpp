/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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

DirectoryJob::~DirectoryJob()
{
}

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
    QNetworkReply *reply = submitGetRequest();
    connect(reply, &QNetworkReply::finished, this, &DirectoryJob::slotDirectoryFinished);
    addStartRestApiInfo(QByteArrayLiteral("DirectoryJob: Ask for server statistics"));
    return true;
}

void DirectoryJob::slotDirectoryFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("DirectoryJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT directoryDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("DirectoryJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

DirectoryJob::DirectoryInfo DirectoryJob::directoryInfo() const
{
    return mDirectoryInfo;
}

void DirectoryJob::setDirectoryInfo(const DirectoryInfo &directoryInfo)
{
    mDirectoryInfo = directoryInfo;
}

QNetworkRequest DirectoryJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Directory);
    QUrlQuery queryUrl;
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
    return searchType != SearchType::Unknown && !pattern.isEmpty();
}

bool DirectoryJob::DirectoryInfo::operator==(const DirectoryJob::DirectoryInfo &other) const
{
    return other.searchType == searchType && other.pattern == pattern;
}
