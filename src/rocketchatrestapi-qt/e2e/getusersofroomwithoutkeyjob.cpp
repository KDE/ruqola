/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getusersofroomwithoutkeyjob.h"

#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QUrlQuery>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
GetUsersOfRoomWithoutKeyJob::GetUsersOfRoomWithoutKeyJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetUsersOfRoomWithoutKeyJob::~GetUsersOfRoomWithoutKeyJob() = default;

bool GetUsersOfRoomWithoutKeyJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("Start GetUsersOfRoomWithoutKey"_ba);

    return true;
}

void GetUsersOfRoomWithoutKeyJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
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

QByteArray GetUsersOfRoomWithoutKeyJob::roomId() const
{
    return mRoomId;
}

void GetUsersOfRoomWithoutKeyJob::setRoomId(const QByteArray &newRoomId)
{
    mRoomId = newRoomId;
}

QNetworkRequest GetUsersOfRoomWithoutKeyJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::E2EGetUsersOfRoomWithoutKey);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(u"rid"_s, QLatin1StringView(mRoomId));
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

bool GetUsersOfRoomWithoutKeyJob::requireHttpAuthentication() const
{
    return true;
}

#include "moc_getusersofroomwithoutkeyjob.cpp"
