/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resete2ekeyjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
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
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("ResetE2EKeyJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT resetE2EKeyDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("ResetE2EKeyJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

const QByteArray &ResetE2EKeyJob::resetUserId() const
{
    return mResetUserId;
}

void ResetE2EKeyJob::setResetUserId(const QByteArray &newResetUserId)
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
    jsonObj["userId"_L1] = QLatin1StringView(mResetUserId);
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool ResetE2EKeyJob::requireTwoFactorAuthentication() const
{
    return true;
}

#include "moc_resete2ekeyjob.cpp"
