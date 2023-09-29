/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customuserstatusdeletejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
CustomUserStatusDeleteJob::CustomUserStatusDeleteJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

CustomUserStatusDeleteJob::~CustomUserStatusDeleteJob() = default;

bool CustomUserStatusDeleteJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("CustomUserStatusDeleteJob::start");
    submitPostRequest(json());

    return true;
}

void CustomUserStatusDeleteJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("CustomUserStatusDeleteJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT userStatusDeletedDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("CustomUserStatusDeleteJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString CustomUserStatusDeleteJob::customUserStatusId() const
{
    return mCustomUserStatusId;
}

void CustomUserStatusDeleteJob::setCustomUserStatusId(const QString &customUserStatusId)
{
    mCustomUserStatusId = customUserStatusId;
}

bool CustomUserStatusDeleteJob::requireHttpAuthentication() const
{
    return true;
}

bool CustomUserStatusDeleteJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mCustomUserStatusId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "CustomUserStatusDeleteJob: mCustomUserStatusId is empty.";
        return false;
    }
    return true;
}

QNetworkRequest CustomUserStatusDeleteJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::CustomUserStatusDelete);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument CustomUserStatusDeleteJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("customUserStatusId")] = mCustomUserStatusId;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_customuserstatusdeletejob.cpp"
