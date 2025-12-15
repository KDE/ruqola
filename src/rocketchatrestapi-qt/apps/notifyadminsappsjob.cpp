/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notifyadminsappsjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
NotifyAdminsAppsJob::NotifyAdminsAppsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

NotifyAdminsAppsJob::~NotifyAdminsAppsJob() = default;

bool NotifyAdminsAppsJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("NotifyAdminsAppsJob::start");
    submitPostRequest(json());

    return true;
}

void NotifyAdminsAppsJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("NotifyAdminsAppsJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT notifyAdminsAppsDone(replyObject["data"_L1].toObject());
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("NotifyAdminsAppsJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

NotifyAdminsAppsJob::NotifyAdminsAppsInfo NotifyAdminsAppsJob::info() const
{
    return mInfo;
}

void NotifyAdminsAppsJob::setInfo(const NotifyAdminsAppsInfo &newInfo)
{
    mInfo = newInfo;
}

bool NotifyAdminsAppsJob::requireHttpAuthentication() const
{
    return true;
}

bool NotifyAdminsAppsJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "NotifyAdminsAppsJob: mInfo is invalid";
        return false;
    }
    return true;
}

QNetworkRequest NotifyAdminsAppsJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::NotifyAdminsApps, RestApiUtil::RestApiUrlExtensionType::Apps);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument NotifyAdminsAppsJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["appId"_L1] = QString::fromLatin1(mInfo.appId);
    jsonObj["appName"_L1] = mInfo.appName;
    jsonObj["appVersion"_L1] = mInfo.appVersion;
    jsonObj["message"_L1] = mInfo.message;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool NotifyAdminsAppsJob::NotifyAdminsAppsInfo::isValid() const
{
    return !appId.isEmpty() && !appName.isEmpty() && !appVersion.isEmpty() && !message.isEmpty();
}

#include "moc_notifyadminsappsjob.cpp"
