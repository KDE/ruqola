/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "markroomasunreadjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
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

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("MarkRoomAsUnReadJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT markAsUnReadDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("MarkRoomAsUnReadJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString MarkRoomAsUnReadJob::objectId() const
{
    return mObjectId;
}

void MarkRoomAsUnReadJob::setObjectId(const QString &objectId)
{
    mObjectId = objectId;
}

QString MarkRoomAsUnReadJob::errorMessage(const QString &str, const QJsonObject &detail)
{
    if (str == QLatin1String("error-action-not-allowed")) {
        return i18n("Unread message not authorized.");
    } else if (str == QLatin1String("error-no-message-for-unread")) {
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
    if (mUnReadObject == Unknown) {
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
    if (mUnReadObject == Room) {
        jsonObj[QLatin1String("roomId")] = mObjectId;
    } else if (mUnReadObject == FromMessage) {
        QJsonObject fromMessage;
        fromMessage[QLatin1String("_id")] = mObjectId;
        jsonObj[QLatin1String("firstUnreadMessage")] = fromMessage;
    }

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_markroomasunreadjob.cpp"
