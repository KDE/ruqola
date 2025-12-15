/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "deletemessagejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
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

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("DeleteMessageJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT deleteMessageDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("DeleteMessageJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
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
    jsonObj["roomId"_L1] = QLatin1StringView(roomId());
    jsonObj["msgId"_L1] = QLatin1StringView(messageId());

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QByteArray DeleteMessageJob::roomId() const
{
    return mRoomId;
}

void DeleteMessageJob::setRoomId(const QByteArray &roomId)
{
    mRoomId = roomId;
}

QByteArray DeleteMessageJob::messageId() const
{
    return mMessageId;
}

void DeleteMessageJob::setMessageId(const QByteArray &t)
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
    return u"Delete Message"_s;
}

QString DeleteMessageJob::errorMessage(const QString &str, const QJsonObject &details)
{
    if (str == "error-action-not-allowed"_L1) {
        return i18n("Message deletion is not allowed.");
    }
    return RestApiAbstractJob::errorMessage(str, details);
}

#include "moc_deletemessagejob.cpp"
