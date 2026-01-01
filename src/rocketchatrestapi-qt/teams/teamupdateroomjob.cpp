/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamupdateroomjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
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
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("TeamUpdateRoomJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT teamUpdateRoomDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("TeamUpdateRoomJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
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
    jsonObj["roomId"_L1] = QLatin1StringView(mRoomId);
    jsonObj["isDefault"_L1] = mIsDefault;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_teamupdateroomjob.cpp"
