/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "settopicdmjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
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
    addStartRestApiInfo("SetTopicDmJob::start"_ba);
    submitPostRequest(json());

    return true;
}

void SetTopicDmJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (checkResponse("SetTopicDmJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT setTopicDmDone();
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
    jsonObj["userId"_L1] = mDirectUserId;
    jsonObj["topic"_L1] = mTopic;
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

#include "moc_settopicdmjob.cpp"
