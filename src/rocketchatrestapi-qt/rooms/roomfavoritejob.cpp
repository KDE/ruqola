/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomfavoritejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <KLocalizedString>

#include <QJsonDocument>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
RoomFavoriteJob::RoomFavoriteJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomFavoriteJob::~RoomFavoriteJob() = default;

bool RoomFavoriteJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RoomFavoriteJob::start");
    submitPostRequest(json());

    return true;
}

void RoomFavoriteJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("RoomFavoriteJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT changeFavoriteDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("RoomFavoriteJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool RoomFavoriteJob::favorite() const
{
    return mFavorite;
}

void RoomFavoriteJob::setFavorite(bool favorite)
{
    mFavorite = favorite;
}

QString RoomFavoriteJob::errorMessage(const QString &str, const QJsonObject &details)
{
    if (str == "error-invalid-subscription"_L1) {
        return i18n("Invalid Subscription");
    }
    return RestApiAbstractJob::errorMessage(str, details);
}

QString RoomFavoriteJob::roomId() const
{
    return mRoomId;
}

void RoomFavoriteJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

bool RoomFavoriteJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomFavoriteJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomFavoriteJob: mRoomId is empty";
        return false;
    }
    return true;
}

QNetworkRequest RoomFavoriteJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsFavorite);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument RoomFavoriteJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["roomId"_L1] = mRoomId;
    jsonObj["favorite"_L1] = mFavorite;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_roomfavoritejob.cpp"
