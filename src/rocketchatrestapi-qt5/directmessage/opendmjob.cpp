/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "opendmjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
OpenDmJob::OpenDmJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

OpenDmJob::~OpenDmJob() = default;

bool OpenDmJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("OpenDmJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &OpenDmJob::slotOpenDmFinished);
    return true;
}

void OpenDmJob::slotOpenDmFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("Create direct message success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT openDmDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("Create direct message Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString OpenDmJob::directUserId() const
{
    return mDirectUserId;
}

void OpenDmJob::setDirectUserId(const QString &userId)
{
    mDirectUserId = userId;
}

bool OpenDmJob::requireHttpAuthentication() const
{
    return true;
}

bool OpenDmJob::canStart() const
{
    if (mDirectUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "OpenDmJob: mDirectUserId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument OpenDmJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("userId")] = mDirectUserId;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest OpenDmJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ImOpen);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
