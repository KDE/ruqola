/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "settopicdmjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
SetTopicDmJob::SetTopicDmJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SetTopicDmJob::~SetTopicDmJob() = default;

bool SetTopicDmJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("SetTopicDmJob::start");
    submitPostRequest(json());

    return true;
}

void SetTopicDmJob::onPostRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("Create direct message success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT setTopicDmDone();
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("Create direct message Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString SetTopicDmJob::directUserId() const
{
    return mDirectUserId;
}

void SetTopicDmJob::setDirectUserId(const QString &userId)
{
    mDirectUserId = userId;
}

bool SetTopicDmJob::requireHttpAuthentication() const
{
    return true;
}

bool SetTopicDmJob::canStart() const
{
    if (mDirectUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetTopicDmJob: mDirectUserId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument SetTopicDmJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("userId")] = mDirectUserId;
    jsonObj[QLatin1String("topic")] = mTopic;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest SetTopicDmJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ImOpen);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
