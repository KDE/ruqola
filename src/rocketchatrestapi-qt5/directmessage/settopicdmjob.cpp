/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &SetTopicDmJob::slotSetTopicDmFinished);
    return true;
}

void SetTopicDmJob::slotSetTopicDmFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("Create direct message success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT setTopicDmDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("Create direct message Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
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
