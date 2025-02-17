/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customuserstatuscreatejob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
CustomUserStatusCreateJob::CustomUserStatusCreateJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

CustomUserStatusCreateJob::~CustomUserStatusCreateJob() = default;

bool CustomUserStatusCreateJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("CustomUserStatusCreateJob::start");
    submitPostRequest(json());

    return true;
}

void CustomUserStatusCreateJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("CustomUserStatusCreateJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT createUserStatusDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("CustomUserStatusCreateJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

CustomUserStatusCreateJob::StatusCreateInfo CustomUserStatusCreateJob::statusCreateInfo() const
{
    return mStatusCreateInfo;
}

void CustomUserStatusCreateJob::setStatusCreateInfo(const StatusCreateInfo &statusCreateInfo)
{
    mStatusCreateInfo = statusCreateInfo;
}

bool CustomUserStatusCreateJob::requireHttpAuthentication() const
{
    return true;
}

bool CustomUserStatusCreateJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mStatusCreateInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "CustomUserStatusCreateJob: mStatusCreateInfo is not valid.";
        return false;
    }
    return true;
}

QNetworkRequest CustomUserStatusCreateJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::CustomUserStatusCreate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument CustomUserStatusCreateJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["name"_L1] = mStatusCreateInfo.name;
    jsonObj["statusType"_L1] = mStatusCreateInfo.statusType;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_customuserstatuscreatejob.cpp"
