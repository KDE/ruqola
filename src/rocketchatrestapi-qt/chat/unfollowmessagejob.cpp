/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "unfollowmessagejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
UnFollowMessageJob::UnFollowMessageJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UnFollowMessageJob::~UnFollowMessageJob() = default;

bool UnFollowMessageJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("UnFollowMessageJob::start");
    submitPostRequest(json());

    return true;
}

void UnFollowMessageJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1StringView("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("UnFollowMessageJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT unFollowMessageDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("UnFollowMessageJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool UnFollowMessageJob::requireHttpAuthentication() const
{
    return true;
}

bool UnFollowMessageJob::canStart() const
{
    if (mMessageId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "UnFollowMessageJob: messageId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument UnFollowMessageJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1StringView("mid")] = QString::fromLatin1(mMessageId);

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QByteArray UnFollowMessageJob::messageId() const
{
    return mMessageId;
}

void UnFollowMessageJob::setMessageId(const QByteArray &t)
{
    mMessageId = t;
}

QNetworkRequest UnFollowMessageJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatUnFollowMessage);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_unfollowmessagejob.cpp"
