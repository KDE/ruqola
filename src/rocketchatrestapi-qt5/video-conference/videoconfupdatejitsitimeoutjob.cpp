/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconfupdatejitsitimeoutjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
VideoConfUpdateJitsiTimeOutJob::VideoConfUpdateJitsiTimeOutJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

VideoConfUpdateJitsiTimeOutJob::~VideoConfUpdateJitsiTimeOutJob() = default;

bool VideoConfUpdateJitsiTimeOutJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("VideoConfUpdateJitsiTimeOutJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &VideoConfUpdateJitsiTimeOutJob::slotUpdateJitsiTimeOut);
    return true;
}

void VideoConfUpdateJitsiTimeOutJob::slotUpdateJitsiTimeOut()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("VideoConfUpdateJitsiTimeOutJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT updateJitsiTimeOutDone(replyObject[QStringLiteral("jitsiTimeout")].toString());
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("VideoConfUpdateJitsiTimeOutJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString VideoConfUpdateJitsiTimeOutJob::roomId() const
{
    return mRoomId;
}

void VideoConfUpdateJitsiTimeOutJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

bool VideoConfUpdateJitsiTimeOutJob::requireHttpAuthentication() const
{
    return true;
}

bool VideoConfUpdateJitsiTimeOutJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "VideoConfUpdateJitsiTimeOutJob: mRoomid is empty";
        return false;
    }
    return true;
}

QNetworkRequest VideoConfUpdateJitsiTimeOutJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::VideoConfJitsi);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument VideoConfUpdateJitsiTimeOutJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = mRoomId;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
