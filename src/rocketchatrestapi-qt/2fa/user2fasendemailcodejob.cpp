/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "user2fasendemailcodejob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
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
    Q_UNUSED(replyErrorString);
    // TODO it reports only email.

    addLoggerInfo("User2FASendEmailCodeJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
    Q_EMIT sendEmailCodeDone();

    // const QJsonObject replyObject = replyJson.object();
    // qDebug() << "replyObject  "<< replyObject;
    // if (replyObject["success"_L1].toBool()) {
    // } else {
    // emitFailedMessage(replyObject, reply);
    // addLoggerWarning("User2FASendEmailCodeJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
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
    jsonObj["emailOrUsername"_L1] = mUsernameOrEmail;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_user2fasendemailcodejob.cpp"
