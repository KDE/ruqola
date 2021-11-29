/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "deleteownaccountjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
DeleteOwnAccountJob::DeleteOwnAccountJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

DeleteOwnAccountJob::~DeleteOwnAccountJob() = default;

bool DeleteOwnAccountJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("DeleteOwnAccountJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &DeleteOwnAccountJob::slotDeleteOwnAccount);
    return true;
}

void DeleteOwnAccountJob::slotDeleteOwnAccount()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("DeleteOwnAccountJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT deleteOwnAccountDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("DeleteOwnAccountJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString DeleteOwnAccountJob::password() const
{
    return mPassword;
}

void DeleteOwnAccountJob::setPassword(const QString &password)
{
    mPassword = password;
}

bool DeleteOwnAccountJob::requireHttpAuthentication() const
{
    return true;
}

bool DeleteOwnAccountJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mPassword.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "DeleteOwnAccountJob: mPassword is empty";
        return false;
    }
    return true;
}

QNetworkRequest DeleteOwnAccountJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersDeleteOwnAccount);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument DeleteOwnAccountJob::json() const
{
    QJsonObject jsonObj;
    const QByteArray sha256pw = RestApiUtil::convertSha256Password(mPassword);
    jsonObj[QLatin1String("password")] = QString::fromLatin1(sha256pw);

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
