/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "markroomasunreadjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
MarkRoomAsUnReadJob::MarkRoomAsUnReadJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

MarkRoomAsUnReadJob::~MarkRoomAsUnReadJob() = default;

bool MarkRoomAsUnReadJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("MarkRoomAsUnReadJob::start");
    submitPostRequest(json());

    return true;
}

void MarkRoomAsUnReadJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("MarkRoomAsUnReadJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT markAsUnReadDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("MarkRoomAsUnReadJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QByteArray MarkRoomAsUnReadJob::objectId() const
{
    return mObjectId;
}

void MarkRoomAsUnReadJob::setObjectId(const QByteArray &objectId)
{
    mObjectId = objectId;
}

QString MarkRoomAsUnReadJob::errorMessage(const QString &str, const QJsonObject &detail)
{
    if (str == "error-action-not-allowed"_L1) {
        return i18n("Unread message not authorized.");
    } else if (str == "error-no-message-for-unread"_L1) {
        return i18n("There are no messages to mark unread.");
    }
    return RestApiAbstractJob::errorMessage(str, detail);
}

MarkRoomAsUnReadJob::MarkAsUnReadObject MarkRoomAsUnReadJob::unReadObject() const
{
    return mUnReadObject;
}

void MarkRoomAsUnReadJob::setUnReadObject(MarkAsUnReadObject type)
{
    mUnReadObject = type;
}

bool MarkRoomAsUnReadJob::requireHttpAuthentication() const
{
    return true;
}

bool MarkRoomAsUnReadJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mUnReadObject == MarkAsUnReadObject::Unknown) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "MarkRoomAsUnReadJob: object type undefined";
        return false;
    }

    if (mObjectId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "MarkRoomAsUnReadJob: mRoomId is empty";
        return false;
    }
    return true;
}

QNetworkRequest MarkRoomAsUnReadJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::SubscriptionsUnRead);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument MarkRoomAsUnReadJob::json() const
{
    QJsonObject jsonObj;
    if (mUnReadObject == MarkAsUnReadObject::Room) {
        jsonObj["roomId"_L1] = QLatin1StringView(mObjectId);
    } else if (mUnReadObject == MarkAsUnReadObject::FromMessage) {
        QJsonObject fromMessage;
        fromMessage["_id"_L1] = QLatin1StringView(mObjectId);
        jsonObj["firstUnreadMessage"_L1] = fromMessage;
    }

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_markroomasunreadjob.cpp"
