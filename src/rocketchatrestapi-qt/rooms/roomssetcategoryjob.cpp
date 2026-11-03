/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomssetcategoryjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonArray>
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
        Q_EMIT roomsSetCategoryDone();
    }
}

QList<QByteArray> RoomsSetCategoryJob::roomIds() const
{
    return mRoomIds;
}

void RoomsSetCategoryJob::setRoomIds(const QList<QByteArray> &newRoomIds)
{
    mRoomIds = newRoomIds;
}

QByteArray RoomsSetCategoryJob::categoryId() const
{
    return mCategoryId;
}

void RoomsSetCategoryJob::setCategoryId(const QByteArray &newCategoryId)
{
    mCategoryId = newCategoryId;
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
    if (mCategoryId.isEmpty() || mRoomIds.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomsSetCategoryJob: mCategoryId or mRoomIds is empty.";
        return false;
    }
    return true;
}

QJsonDocument RoomsSetCategoryJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["category"_L1] = QString::fromLatin1(mCategoryId);
    QJsonArray roomIdsArray;
    for (const QByteArray &ba : mRoomIds) {
        roomIdsArray.append(QString::fromLatin1(ba));
    }
    jsonObj["roomIds"_L1] = roomIdsArray;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest RoomsSetCategoryJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsSetCategory, RestApiUtil::RestApiUrlExtensionType::Experimental);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_roomssetcategoryjob.cpp"
