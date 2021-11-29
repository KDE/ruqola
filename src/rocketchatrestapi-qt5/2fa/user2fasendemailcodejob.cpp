/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "user2fasendemailcodejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
User2FASendEmailCodeJob::User2FASendEmailCodeJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

User2FASendEmailCodeJob::~User2FASendEmailCodeJob() = default;

bool User2FASendEmailCodeJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("User2FASendEmailCodeJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &User2FASendEmailCodeJob::slotSendEmailCode);
    return true;
}

void User2FASendEmailCodeJob::slotSendEmailCode()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        qDebug() << "data " << data;
        // TODO it reports only email.
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        addLoggerInfo(QByteArrayLiteral("User2FASendEmailCodeJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT sendEmailCodeDone();

        //        const QJsonObject replyObject = replyJson.object();
        //        qDebug() << "replyObject  "<< replyObject;
        //        if (replyObject[QStringLiteral("success")].toBool()) {
        //        } else {
        //            emitFailedMessage(replyObject, reply);
        //            addLoggerWarning(QByteArrayLiteral("User2FASendEmailCodeJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        //        }
        reply->deleteLater();
    }
    deleteLater();
}

QString User2FASendEmailCodeJob::usernameOrEmail() const
{
    return mUsernameOrEmail;
}

void User2FASendEmailCodeJob::setUsernameOrEmail(const QString &usernameOrEmail)
{
    mUsernameOrEmail = usernameOrEmail;
}

bool User2FASendEmailCodeJob::requireHttpAuthentication() const
{
    return false;
}

bool User2FASendEmailCodeJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mUsernameOrEmail.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "mUsernameOrEmail is empty";
        return false;
    }
    return true;
}

QNetworkRequest User2FASendEmailCodeJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Users2FASendEmailCode);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument User2FASendEmailCodeJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("emailOrUsername")] = mUsernameOrEmail;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
