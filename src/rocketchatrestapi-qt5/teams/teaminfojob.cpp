/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teaminfojob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
TeamInfoJob::TeamInfoJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

TeamInfoJob::~TeamInfoJob() = default;

bool TeamInfoJob::requireHttpAuthentication() const
{
    return true;
}

bool TeamInfoJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start TeamInfoJob job";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = submitGetRequest();
    connect(reply, &QNetworkReply::finished, this, &TeamInfoJob::slotTeamInfoFinished);
    addStartRestApiInfo(QByteArrayLiteral("TeamInfoJob: ask list of rooms in team"));
    return true;
}

void TeamInfoJob::slotTeamInfoFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("TeamInfoJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT teamInfoDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("TeamInfoJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString TeamInfoJob::teamId() const
{
    return mTeamId;
}

void TeamInfoJob::setTeamId(const QString &teamId)
{
    mTeamId = teamId;
}

QNetworkRequest TeamInfoJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::TeamsInfo);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("teamId"), mTeamId);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

bool TeamInfoJob::canStart() const
{
    if (mTeamId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TeamInfoJob: mTeamId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}
