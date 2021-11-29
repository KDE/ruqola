/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "getusernamesuggestionjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
GetUsernameSuggestionJob::GetUsernameSuggestionJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetUsernameSuggestionJob::~GetUsernameSuggestionJob() = default;

bool GetUsernameSuggestionJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

bool GetUsernameSuggestionJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    QNetworkReply *reply = submitGetRequest();
    connect(reply, &QNetworkReply::finished, this, &GetUsernameSuggestionJob::slotGetUsernameSuggestion);
    addStartRestApiInfo("GetUsernameSuggestionJob start");

    return true;
}

void GetUsernameSuggestionJob::slotGetUsernameSuggestion()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("GetUsernameSuggestionJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            const QString result = replyObject[QStringLiteral("result")].toString();
            Q_EMIT getUsernameSuggestionDone(result);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("GetUsernameSuggestionJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QNetworkRequest GetUsernameSuggestionJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersGetUsernameSuggestion);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    return request;
}

bool GetUsernameSuggestionJob::requireHttpAuthentication() const
{
    return true;
}
