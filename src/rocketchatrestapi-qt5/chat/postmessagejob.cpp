/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "postmessagejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
PostMessageJob::PostMessageJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

PostMessageJob::~PostMessageJob() = default;

bool PostMessageJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("PostMessageJob::start");
    submitPostRequest(json());

    return true;
}

bool PostMessageJob::requireHttpAuthentication() const
{
    return true;
}

void PostMessageJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("PostMessageJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT postMessageDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("PostMessageJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QNetworkRequest PostMessageJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatPostMessage);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QString PostMessageJob::text() const
{
    return mText;
}

void PostMessageJob::setText(const QString &text)
{
    mText = text;
}

QString PostMessageJob::roomId() const
{
    return mRoomId;
}

void PostMessageJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

bool PostMessageJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    // It can be optional!
    if (mText.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Text is empty";
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "roomId is not defined";
        return false;
    }
    return true;
}

QJsonDocument PostMessageJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = mRoomId;
    jsonObj[QLatin1String("text")] = mText;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString PostMessageJob::generateErrorMessage(const QString &errorStr) const
{
    if (errorStr == QLatin1String("room_is_blocked")) {
        return i18n("This room is blocked");
    } else if (errorStr == QLatin1String("You_have_been_muted")) {
        return i18n("You have been muted and cannot speak in this room");
    }
    return RestApiAbstractJob::generateErrorMessage(errorStr);
}
