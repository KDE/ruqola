/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getusersofroomwithoutkey.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
GetUsersOfRoomWithoutKey::GetUsersOfRoomWithoutKey(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetUsersOfRoomWithoutKey::~GetUsersOfRoomWithoutKey() = default;

bool GetUsersOfRoomWithoutKey::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("Start GetUsersOfRoomWithoutKey");

    return true;
}

void GetUsersOfRoomWithoutKey::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("GetUsersOfRoomWithoutKey: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT fetchMyKeysDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("GetUsersOfRoomWithoutKey: problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QByteArray GetUsersOfRoomWithoutKey::roomId() const
{
    return mRoomId;
}

void GetUsersOfRoomWithoutKey::setRoomId(const QByteArray &newRoomId)
{
    mRoomId = newRoomId;
}

QNetworkRequest GetUsersOfRoomWithoutKey::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::E2EfetchMyKeys);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    return request;
}

bool GetUsersOfRoomWithoutKey::requireHttpAuthentication() const
{
    return true;
}

#include "moc_getusersofroomwithoutkey.cpp"
