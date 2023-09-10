/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "removeothertokensjob.h"
#include "restapimethod.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
RemoveOtherTokensJob::RemoveOtherTokensJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RemoveOtherTokensJob::~RemoveOtherTokensJob() = default;

bool RemoveOtherTokensJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RemoveOtherTokensJob::start");
    submitPostRequest(json());
    return true;
}

void RemoveOtherTokensJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("RemoveOtherTokensJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT removeOtherTokensDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("RemoveOtherTokensJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool RemoveOtherTokensJob::requireHttpAuthentication() const
{
    return true;
}

bool RemoveOtherTokensJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QNetworkRequest RemoveOtherTokensJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersRemoveOtherTokens);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument RemoveOtherTokensJob::json() const
{
    return {};
}

#include "moc_removeothertokensjob.cpp"
