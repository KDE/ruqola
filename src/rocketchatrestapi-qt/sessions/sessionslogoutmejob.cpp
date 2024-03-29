/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sessionslogoutmejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
SessionsLogoutMeJob::SessionsLogoutMeJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SessionsLogoutMeJob::~SessionsLogoutMeJob() = default;

bool SessionsLogoutMeJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("SessionsLogoutMeJob::start");
    submitPostRequest(json());

    return true;
}

void SessionsLogoutMeJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1StringView("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("SessionsLogoutMeJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT logoutMeDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("SessionsLogoutMeJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

const QByteArray &SessionsLogoutMeJob::sessionId() const
{
    return mSessionId;
}

void SessionsLogoutMeJob::setSessionId(const QByteArray &newSessionId)
{
    mSessionId = newSessionId;
}

bool SessionsLogoutMeJob::requireHttpAuthentication() const
{
    return true;
}

bool SessionsLogoutMeJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mSessionId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SessionsLogoutMeJob: mSessionId is empty";
        return false;
    }
    return true;
}

QNetworkRequest SessionsLogoutMeJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::SessionsLogoutMe);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument SessionsLogoutMeJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1StringView("sessionId")] = QString::fromLatin1(mSessionId);
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_sessionslogoutmejob.cpp"
