/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getusersofroomwithoutkey.h"

#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
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
        Q_EMIT getUsersOfRoomWithoutKeyDone(replyObject);
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
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::E2EGetUsersOfRoomWithoutKey);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(u"roomId"_s, QLatin1StringView(mRoomId));
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addAuthRawHeader(request);
    return request;
}

bool GetUsersOfRoomWithoutKey::requireHttpAuthentication() const
{
    return true;
}

#include "moc_getusersofroomwithoutkey.cpp"
