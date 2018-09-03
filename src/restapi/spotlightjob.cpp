/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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
#include "restapi_debug.h"
#include "restapimethod.h"
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
        qCWarning(RESTAPI_LOG) << "Impossible to start SpotlightJob job";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = mNetworkAccessManager->get(request());
    addLoggerInfo(QByteArrayLiteral("SpotlightJob start"));
    connect(reply, &QNetworkReply::finished, this, &SpotlightJob::slotSpotlightDone);

    return true;
}

void SpotlightJob::slotSpotlightDone()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();
        addLoggerInfo(QByteArrayLiteral("SpotlightJob done: ") + replyJson.toJson(QJsonDocument::Indented));
        if (replyObject[QStringLiteral("success")].toBool()) {
            qCDebug(RESTAPI_LOG) << "SpotlightJob::slotSpotlightDone: success" << data;
            Q_EMIT spotlightDone(replyObject);
        } else {
            qCWarning(RESTAPI_LOG) <<" Problem when we tried calling spotlight method" << data;
        }
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
        qCWarning(RESTAPI_LOG) << "SpotlightJob: searchpattern is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(RESTAPI_LOG) << "Impossible to start SpotlightJob job";
        return false;
    }
    return true;
}
