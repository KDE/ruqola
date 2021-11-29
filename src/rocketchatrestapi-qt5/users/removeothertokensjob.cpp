/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "removeothertokensjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

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
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &RemoveOtherTokensJob::slotRemoveOtherTokens);
    return true;
}

void RemoveOtherTokensJob::slotRemoveOtherTokens()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("RemoveOtherTokensJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT removeOtherTokensDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("RemoveOtherTokensJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
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
