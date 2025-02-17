/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "opendmjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
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
    submitPostRequest(json());

    return true;
}

void OpenDmJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("Create direct message success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        qDebug() << " replyJson " << replyJson;
        Q_EMIT openDmDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("Create direct message Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
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
    jsonObj["roomId"_L1] = mDirectUserId;
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

#include "moc_opendmjob.cpp"
