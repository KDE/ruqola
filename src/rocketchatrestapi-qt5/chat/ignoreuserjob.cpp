/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "ignoreuserjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
IgnoreUserJob::IgnoreUserJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

IgnoreUserJob::~IgnoreUserJob() = default;

bool IgnoreUserJob::requireHttpAuthentication() const
{
    return true;
}

bool IgnoreUserJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ignore user job";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = submitGetRequest();
    connect(reply, &QNetworkReply::finished, this, &IgnoreUserJob::slotIgnoreUserFinished);
    addStartRestApiInfo(QByteArrayLiteral("IgnoreUserJob: ignore user starting"));
    return true;
}

void IgnoreUserJob::slotIgnoreUserFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("IgnoreUserJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT ignoreUserDone(replyObject, mRoomId);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("Problem when we tried to ignore user message: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool IgnoreUserJob::ignore() const
{
    return mIgnore;
}

void IgnoreUserJob::setIgnore(bool ignore)
{
    mIgnore = ignore;
}

QString IgnoreUserJob::ignoreUserId() const
{
    return mIgnoreUserId;
}

void IgnoreUserJob::setIgnoreUserId(const QString &ignoreUserId)
{
    mIgnoreUserId = ignoreUserId;
}

QString IgnoreUserJob::roomId() const
{
    return mRoomId;
}

void IgnoreUserJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest IgnoreUserJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatIgnoreUser);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("rid"), mRoomId);
    queryUrl.addQueryItem(QStringLiteral("userId"), mIgnoreUserId);
    queryUrl.addQueryItem(QStringLiteral("ignore"), mIgnore ? QStringLiteral("true") : QStringLiteral("false"));
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

bool IgnoreUserJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomId is empty";
        return false;
    }
    if (mIgnoreUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ignore user id is empty";
        return false;
    }
    return true;
}
