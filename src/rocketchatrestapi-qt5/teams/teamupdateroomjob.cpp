/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "teamupdateroomjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
TeamUpdateRoomJob::TeamUpdateRoomJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

TeamUpdateRoomJob::~TeamUpdateRoomJob() = default;

bool TeamUpdateRoomJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("TeamUpdateRoomJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &TeamUpdateRoomJob::slotTeamUpdateRoomFinished);
    return true;
}

void TeamUpdateRoomJob::slotTeamUpdateRoomFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("TeamUpdateRoomJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT teamUpdateRoomDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("TeamUpdateRoomJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool TeamUpdateRoomJob::isDefault() const
{
    return mIsDefault;
}

void TeamUpdateRoomJob::setIsDefault(bool isDefault)
{
    mIsDefault = isDefault;
}

QString TeamUpdateRoomJob::roomId() const
{
    return mRoomId;
}

void TeamUpdateRoomJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

bool TeamUpdateRoomJob::requireHttpAuthentication() const
{
    return true;
}

bool TeamUpdateRoomJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TeamUpdateRoomJob: mRoomId is empty";
        return false;
    }
    return true;
}

QNetworkRequest TeamUpdateRoomJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::TeamsUpdateRoom);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument TeamUpdateRoomJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = mRoomId;
    jsonObj[QLatin1String("isDefault")] = mIsDefault;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
