/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "setstatusjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>

#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
SetStatusJob::SetStatusJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SetStatusJob::~SetStatusJob() = default;

bool SetStatusJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("SetStatusJob::start");
    submitPostRequest(json());
    return true;
}

void SetStatusJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QLatin1StringView("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("SetStatusJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT setStatusDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("SetStatusJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString SetStatusJob::statusUserId() const
{
    return mStatusUserId;
}

void SetStatusJob::setStatusUserId(const QString &statusUserId)
{
    mStatusUserId = statusUserId;
}

SetStatusJob::StatusType SetStatusJob::status() const
{
    return mStatus;
}

void SetStatusJob::setStatus(StatusType status)
{
    mStatus = status;
}

QString SetStatusJob::statusMessage() const
{
    return mStatusMessage;
}

void SetStatusJob::setStatusMessage(const QString &statusMessage)
{
    mStatusMessage = statusMessage;
}

bool SetStatusJob::requireHttpAuthentication() const
{
    return true;
}

bool SetStatusJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mStatusUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetStatusJob: mUserId is empty";
        return false;
    }
    if (mStatus == SetStatusJob::Unknown) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetStatusJob: mStatus is not defined";
        return false;
    }
    return true;
}

QNetworkRequest SetStatusJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersSetStatus);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument SetStatusJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1StringView("userId")] = mStatusUserId;
    QString statusType;
    switch (mStatus) {
    case OnLine:
        statusType = QStringLiteral("online");
        break;
    case Away:
        statusType = QStringLiteral("away");
        break;
    case Offline:
        statusType = QStringLiteral("offline");
        break;
    case Busy:
        statusType = QStringLiteral("busy");
        break;
    case Unknown:
        break;
    }
    jsonObj[QLatin1StringView("status")] = statusType;
    jsonObj[QLatin1StringView("message")] = mStatusMessage;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString SetStatusJob::errorMessage(const QString &str, const QJsonObject &detail)
{
    if (str == QLatin1StringView("error-status-not-allowed")) {
        return i18n("Invisible status is disabled");
    }
    return RestApiAbstractJob::errorMessage(str, detail);
}

#include "moc_setstatusjob.cpp"
