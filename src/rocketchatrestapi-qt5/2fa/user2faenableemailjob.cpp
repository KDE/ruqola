/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "user2faenableemailjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
User2FAEnableEmailJob::User2FAEnableEmailJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

User2FAEnableEmailJob::~User2FAEnableEmailJob() = default;

bool User2FAEnableEmailJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("User2FAEnableEmail::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &User2FAEnableEmailJob::slotEnabledEmail);
    return true;
}

void User2FAEnableEmailJob::slotEnabledEmail()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("User2FAEnableEmail: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT enableEmailDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("User2FAEnableEmail: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool User2FAEnableEmailJob::requireHttpAuthentication() const
{
    return true;
}

bool User2FAEnableEmailJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QNetworkRequest User2FAEnableEmailJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Users2FAEnableEmail);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument User2FAEnableEmailJob::json() const
{
    QJsonObject jsonObj;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
