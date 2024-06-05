/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notifyadminsappsjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
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
        addLoggerInfo(QByteArrayLiteral("NotifyAdminsAppsJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT notifyAdminsAppsDone(replyObject["data"_L1].toObject());
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("NotifyAdminsAppsJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString NotifyAdminsAppsJob::callId() const
{
    return mCallId;
}

void NotifyAdminsAppsJob::setCallId(const QString &newCallId)
{
    mCallId = newCallId;
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
    if (mCallId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "NotifyAdminsAppsJob: mCallId is invalid";
        return false;
    }
    return true;
}

QNetworkRequest NotifyAdminsAppsJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::NotifyAdminsApps);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument NotifyAdminsAppsJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["callId"_L1] = mCallId;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_notifyadminsappsjob.cpp"
