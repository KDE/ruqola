/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

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

SpotlightJob::~SpotlightJob()
{
}

bool SpotlightJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start SpotlightJob job";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = submitGetRequest();
    addStartRestApiInfo(QByteArrayLiteral("SpotlightJob start"));
    connect(reply, &QNetworkReply::finished, this, &SpotlightJob::slotSpotlightDone);

    return true;
}

void SpotlightJob::slotSpotlightDone()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("SpotlightJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT spotlightDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("SpotlightJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
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
