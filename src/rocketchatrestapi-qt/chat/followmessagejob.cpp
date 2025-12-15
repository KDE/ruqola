/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "followmessagejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
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

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("FollowMessageJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT followMessageDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("FollowMessageJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
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
    jsonObj["mid"_L1] = QLatin1StringView(mMessageId);

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
