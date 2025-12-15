/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsnameexistsjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
RoomsNameExistsJob::RoomsNameExistsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomsNameExistsJob::~RoomsNameExistsJob() = default;

bool RoomsNameExistsJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomsNameExistsJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomName.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomsNameExistsJob: RoomName is empty";
        return false;
    }
    return true;
}

bool RoomsNameExistsJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start RoomsNameExistsJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("RoomsNameExistsJob: Ask if room name already exists"_ba);
    return true;
}

void RoomsNameExistsJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("RoomsNameExistsJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT roomNameExistsDone(replyObject["exists"_L1].toBool());
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("RoomsNameExistsJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

const QString &RoomsNameExistsJob::roomName() const
{
    return mRoomName;
}

void RoomsNameExistsJob::setRoomName(const QString &newRoomId)
{
    mRoomName = newRoomId;
}

QNetworkRequest RoomsNameExistsJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsNameExists);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(u"roomName"_s, mRoomName);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_roomsnameexistsjob.cpp"
