/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "deletemessagejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
DeleteMessageJob::DeleteMessageJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

DeleteMessageJob::~DeleteMessageJob() = default;

bool DeleteMessageJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("DeleteMessageJob::start");
    submitPostRequest(json());

    return true;
}

void DeleteMessageJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("DeleteMessageJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT deleteMessageDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("DeleteMessageJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool DeleteMessageJob::requireHttpAuthentication() const
{
    return true;
}

bool DeleteMessageJob::canStart() const
{
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "DeleteMessageJob: roomId is empty";
        return false;
    }
    if (mMessageId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "DeleteMessageJob: messageId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument DeleteMessageJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = roomId();
    jsonObj[QLatin1String("msgId")] = messageId();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString DeleteMessageJob::roomId() const
{
    return mRoomId;
}

void DeleteMessageJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QString DeleteMessageJob::messageId() const
{
    return mMessageId;
}

void DeleteMessageJob::setMessageId(const QString &t)
{
    mMessageId = t;
}

QNetworkRequest DeleteMessageJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatDelete);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QString DeleteMessageJob::jobName() const
{
    return QStringLiteral("Delete Message");
}

QString DeleteMessageJob::errorMessage(const QString &str, const QJsonObject &details)
{
    if (str == QLatin1String("error-action-not-allowed")) {
        return i18n("Message deletion is not allowed.");
    }
    return RestApiAbstractJob::errorMessage(str, details);
}

#include "moc_deletemessagejob.cpp"
