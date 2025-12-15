/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customuserstatusupdatejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
CustomUserStatusUpdateJob::CustomUserStatusUpdateJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

CustomUserStatusUpdateJob::~CustomUserStatusUpdateJob() = default;

bool CustomUserStatusUpdateJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("CustomUserStatusUpdateJob::start");
    submitPostRequest(json());

    return true;
}

void CustomUserStatusUpdateJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("CustomUserStatusUpdateJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT customUserUpdateDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("CustomUserStatusUpdateJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

CustomUserStatusUpdateJob::StatusUpdateInfo CustomUserStatusUpdateJob::statusUpdateInfo() const
{
    return mStatusUpdateInfo;
}

void CustomUserStatusUpdateJob::setStatusUpdateInfo(const StatusUpdateInfo &statusUpdateInfo)
{
    mStatusUpdateInfo = statusUpdateInfo;
}

bool CustomUserStatusUpdateJob::requireHttpAuthentication() const
{
    return true;
}

bool CustomUserStatusUpdateJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mStatusUpdateInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "CustomUserStatusUpdateJob: CustomUserStatusUpdateJob is not valid.";
        return false;
    }
    return true;
}

QNetworkRequest CustomUserStatusUpdateJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::CustomUserStatusUpdate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument CustomUserStatusUpdateJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["_id"_L1] = QLatin1StringView(mStatusUpdateInfo.identifier);
    jsonObj["name"_L1] = mStatusUpdateInfo.name;
    jsonObj["statusType"_L1] = mStatusUpdateInfo.statusType;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_customuserstatusupdatejob.cpp"
