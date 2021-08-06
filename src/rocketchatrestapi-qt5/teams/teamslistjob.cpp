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

#include "teamslistjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
TeamsListJob::TeamsListJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

TeamsListJob::~TeamsListJob()
{
}

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
    QNetworkReply *reply = submitGetRequest();
    connect(reply, &QNetworkReply::finished, this, &TeamsListJob::slotTeamsListFinished);
    addStartRestApiInfo(QByteArrayLiteral("TeamsListJob: ask users list"));
    return true;
}

void TeamsListJob::slotTeamsListFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("TeamsListJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT teamsListDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("TeamsListJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
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
    map.insert(QStringLiteral("text"), mPattern);
    parameters.setCustom(map);
    setQueryParameters(parameters);
}

QNetworkRequest TeamsListJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::TeamsList);
    QUrlQuery queryUrl;
    // TODO addQueryUrl(url);
    // initialQueryParameters();
    addQueryParameter(queryUrl);
    // TODO verify it url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

bool TeamsListJob::hasQueryParameterSupport() const
{
    return true;
}

bool TeamsListJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}
