/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsdeletejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
RoomsDeleteJob::RoomsDeleteJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomsDeleteJob::~RoomsDeleteJob() = default;

bool RoomsDeleteJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RoomsDeleteJob::start");
    submitPostRequest(json());

    return true;
}

void RoomsDeleteJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("RoomsDeleteJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT roomDeleteDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("RoomsDeleteJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString RoomsDeleteJob::roomId() const
{
    return mRoomId;
}

void RoomsDeleteJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

bool RoomsDeleteJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomsDeleteJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomsDeleteJob: mRoomId is empty";
        return false;
    }
    return true;
}

QNetworkRequest RoomsDeleteJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsDelete);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument RoomsDeleteJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["roomId"_L1] = mRoomId;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_roomsdeletejob.cpp"
