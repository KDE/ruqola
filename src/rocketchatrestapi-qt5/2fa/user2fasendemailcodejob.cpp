/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    submitPostRequest(json());
    return true;
}

void User2FASendEmailCodeJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    // TODO it reports only email.

    addLoggerInfo(QByteArrayLiteral("User2FASendEmailCodeJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
    Q_EMIT sendEmailCodeDone();

    // const QJsonObject replyObject = replyJson.object();
    // qDebug() << "replyObject  "<< replyObject;
    // if (replyObject[QStringLiteral("success")].toBool()) {
    // } else {
    // emitFailedMessage(replyObject, reply);
    // addLoggerWarning(QByteArrayLiteral("User2FASendEmailCodeJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    // }
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

#include "moc_user2fasendemailcodejob.cpp"
