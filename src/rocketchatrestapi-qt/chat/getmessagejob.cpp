/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getmessagejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
GetMessageJob::GetMessageJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetMessageJob::~GetMessageJob() = default;

bool GetMessageJob::requireHttpAuthentication() const
{
    return true;
}

bool GetMessageJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start get message job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("GetMessageJob: get message starting"_ba);
    return true;
}

void GetMessageJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("GetMessageJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT getMessageDone(replyObject, mMessageId, mRoomId);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("GetMessageJob: Problem when we tried to get message : "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QByteArray GetMessageJob::roomId() const
{
    return mRoomId;
}

void GetMessageJob::setRoomId(const QByteArray &roomId)
{
    mRoomId = roomId;
}

QByteArray GetMessageJob::messageId() const
{
    return mMessageId;
}

void GetMessageJob::setMessageId(const QByteArray &messageId)
{
    mMessageId = messageId;
}

QNetworkRequest GetMessageJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatGetMessage);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(u"msgId"_s, QLatin1StringView(mMessageId));
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

bool GetMessageJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mMessageId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "MessageId is empty";
        return false;
    }
    return true;
}

QString GetMessageJob::errorMessage(const QString &str, const QJsonObject &details)
{
    if (str == "error-not-allowed"_L1) {
        if (mRoomId.isEmpty()) {
            return i18n("Not allowed to get message %1", QLatin1StringView(mMessageId));
        }
        return i18n("Not allowed to get message %2 in room %1", QLatin1StringView(mRoomId), QLatin1StringView(mMessageId));
    }
    return RestApiAbstractJob::errorMessage(str, details);
}
#include "moc_getmessagejob.cpp"
