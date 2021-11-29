/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "teamdeletejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <QNetworkReply>
using namespace RocketChatRestApi;
TeamDeleteJob::TeamDeleteJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

TeamDeleteJob::~TeamDeleteJob() = default;

bool TeamDeleteJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("TeamDeleteJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &TeamDeleteJob::slotDeleteTeamFinished);
    return true;
}

void TeamDeleteJob::slotDeleteTeamFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("TeamDeleteJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT deleteTeamDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("TeamDeleteJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QStringList TeamDeleteJob::roomsId() const
{
    return mRoomToDeleteId;
}

void TeamDeleteJob::setRoomsId(const QStringList &roomsId)
{
    mRoomToDeleteId = roomsId;
}

QString TeamDeleteJob::teamId() const
{
    return mTeamId;
}

void TeamDeleteJob::setTeamId(const QString &teamId)
{
    mTeamId = teamId;
}

bool TeamDeleteJob::requireHttpAuthentication() const
{
    return true;
}

bool TeamDeleteJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mTeamId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TeamDeleteJob: mTeamId is empty";
        return false;
    }
    return true;
}

QNetworkRequest TeamDeleteJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::TeamsDelete);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument TeamDeleteJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomsToRemove")] = QJsonArray::fromStringList(mRoomToDeleteId);
    jsonObj[QLatin1String("teamId")] = mTeamId;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
