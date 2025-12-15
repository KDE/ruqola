/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "postmessagejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
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

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("PostMessageJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT postMessageDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("PostMessageJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
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

QList<QByteArray> PostMessageJob::roomIds() const
{
    return mRoomIds;
}

void PostMessageJob::setRoomIds(const QList<QByteArray> &roomIds)
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
        jsonObj["roomId"_L1] = QLatin1StringView(mRoomIds.at(0));
    } else {
        QStringList lst;
        lst.reserve(mRoomIds.count());
        for (const QByteArray &b : std::as_const(mRoomIds)) {
            lst.append(QLatin1StringView(b));
        }
        jsonObj["roomId"_L1] = QJsonArray::fromStringList(lst);
    }
    jsonObj["text"_L1] = mText;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    // qDebug() << " postData " << postData;
    return postData;
}

QString PostMessageJob::generateErrorMessage(const QString &errorStr) const
{
    if (errorStr == "room_is_blocked"_L1) {
        return i18n("This room is blocked");
    } else if (errorStr == "You_have_been_muted"_L1) {
        return i18n("You have been muted and cannot speak in this room");
    } else if (errorStr == "invalid-channel"_L1) {
        return i18n("Invalid channel");
    }
    return RestApiAbstractJob::generateErrorMessage(errorStr);
}

QString PostMessageJob::errorMessage(const QString &str, const QJsonObject &details)
{
    if (str == "invalid-channel"_L1) {
        return i18n("Invalid channel");
    }
    return RestApiAbstractJob::errorMessage(str, details);
}

#include "moc_postmessagejob.cpp"
