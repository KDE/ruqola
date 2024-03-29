/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamupdateroomjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
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
    submitPostRequest(json());

    return true;
}

void TeamUpdateRoomJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QLatin1StringView("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("TeamUpdateRoomJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT teamUpdateRoomDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("TeamUpdateRoomJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool TeamUpdateRoomJob::isDefault() const
{
    return mIsDefault;
}

void TeamUpdateRoomJob::setIsDefault(bool isDefault)
{
    mIsDefault = isDefault;
}

QByteArray TeamUpdateRoomJob::roomId() const
{
    return mRoomId;
}

void TeamUpdateRoomJob::setRoomId(const QByteArray &roomId)
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
    jsonObj[QLatin1StringView("roomId")] = QString::fromLatin1(mRoomId);
    jsonObj[QLatin1StringView("isDefault")] = mIsDefault;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_teamupdateroomjob.cpp"
