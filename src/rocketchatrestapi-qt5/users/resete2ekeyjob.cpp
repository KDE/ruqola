/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resete2ekeyjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ResetE2EKeyJob::ResetE2EKeyJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ResetE2EKeyJob::~ResetE2EKeyJob() = default;

bool ResetE2EKeyJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ResetE2EKeyJob::start");
    submitPostRequest(json());
    return true;
}

void ResetE2EKeyJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ResetE2EKeyJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT resetE2EKeyDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ResetE2EKeyJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

const QString &ResetE2EKeyJob::resetUserId() const
{
    return mResetUserId;
}

void ResetE2EKeyJob::setResetUserId(const QString &newResetUserId)
{
    mResetUserId = newResetUserId;
}

bool ResetE2EKeyJob::requireHttpAuthentication() const
{
    return true;
}

bool ResetE2EKeyJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mResetUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ResetE2EKeyJob: mResetUserId is empty";
        return false;
    }
    return true;
}

QNetworkRequest ResetE2EKeyJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersResetE2EKey);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument ResetE2EKeyJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QStringLiteral("userId")] = mResetUserId;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool ResetE2EKeyJob::requireTwoFactorAuthentication() const
{
    return true;
}
