/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomssetcategoryjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
RoomsSetCategoryJob::RoomsSetCategoryJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomsSetCategoryJob::~RoomsSetCategoryJob() = default;

bool RoomsSetCategoryJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RoomsSetCategoryJob::start"_ba);
    submitPostRequest(json());

    return true;
}

void RoomsSetCategoryJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (checkResponse("RoomsSetCategoryJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT roomsBanUserDone();
    }
}

QString RoomsSetCategoryJob::userName() const
{
    return mUserName;
}

void RoomsSetCategoryJob::setUserName(const QString &newUserName)
{
    mUserName = newUserName;
}

QByteArray RoomsSetCategoryJob::roomId() const
{
    return mRoomId;
}

void RoomsSetCategoryJob::setRoomId(const QByteArray &newRoomId)
{
    mRoomId = newRoomId;
}

bool RoomsSetCategoryJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomsSetCategoryJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mUserName.isEmpty() || mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomsSetCategoryJob: mUserName or mRoomId is empty.";
        return false;
    }

    return true;
}

QJsonDocument RoomsSetCategoryJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["roomId"_L1] = QString::fromLatin1(mRoomId);
    jsonObj["username"_L1] = mUserName;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest RoomsSetCategoryJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsBanUser);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_roomssetcategoryjob.cpp"
