/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "followmessagejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
FollowMessageJob::FollowMessageJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

FollowMessageJob::~FollowMessageJob() = default;

bool FollowMessageJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("FollowMessageJob::start");
    submitPostRequest(json());

    return true;
}

void FollowMessageJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1StringView("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("FollowMessageJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT followMessageDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("FollowMessageJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool FollowMessageJob::requireHttpAuthentication() const
{
    return true;
}

bool FollowMessageJob::canStart() const
{
    if (mMessageId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "FollowMessageJob: messageId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument FollowMessageJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1StringView("mid")] = QString::fromLatin1(mMessageId);

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QByteArray FollowMessageJob::messageId() const
{
    return mMessageId;
}

void FollowMessageJob::setMessageId(const QByteArray &t)
{
    mMessageId = t;
}

QNetworkRequest FollowMessageJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatFollowMessage);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_followmessagejob.cpp"
