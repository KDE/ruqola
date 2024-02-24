/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "postmessagejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonArray>
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

    if (replyObject[QLatin1String("success")].toBool()) {
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

QStringList PostMessageJob::roomIds() const
{
    return mRoomIds;
}

void PostMessageJob::setRoomIds(const QStringList &roomIds)
{
    mRoomIds = roomIds;
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
    if (mRoomIds.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "roomId is not defined";
        return false;
    }
    return true;
}

QJsonDocument PostMessageJob::json() const
{
    QJsonObject jsonObj;
    if (mRoomIds.count() == 1) {
        // Make sure to not break old RC server
        jsonObj[QLatin1String("roomId")] = mRoomIds.at(0);
    } else {
        jsonObj[QLatin1String("roomId")] = QJsonArray::fromStringList(mRoomIds);
    }
    jsonObj[QLatin1String("text")] = mText;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    // qDebug() << " postData " << postData;
    return postData;
}

QString PostMessageJob::generateErrorMessage(const QString &errorStr) const
{
    if (errorStr == QLatin1String("room_is_blocked")) {
        return i18n("This room is blocked");
    } else if (errorStr == QLatin1String("You_have_been_muted")) {
        return i18n("You have been muted and cannot speak in this room");
    } else if (errorStr == QLatin1String("invalid-channel")) {
        return i18n("Invalid channel");
    }
    return RestApiAbstractJob::generateErrorMessage(errorStr);
}

QString PostMessageJob::errorMessage(const QString &str, const QJsonObject &details)
{
    if (str == QLatin1String("invalid-channel")) {
        return i18n("Invalid channel");
    }
    return RestApiAbstractJob::errorMessage(str, details);
}

#include "moc_postmessagejob.cpp"
