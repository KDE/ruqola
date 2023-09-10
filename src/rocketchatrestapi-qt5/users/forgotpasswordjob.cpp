/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "forgotpasswordjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ForgotPasswordJob::ForgotPasswordJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ForgotPasswordJob::~ForgotPasswordJob() = default;

bool ForgotPasswordJob::canStart() const
{
    if (mEmail.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Email is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

bool ForgotPasswordJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ForgotPasswordJob::start");
    submitPostRequest(json());
    return true;
}

void ForgotPasswordJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ForgotPasswordJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT forgotPasswordDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ForgotPasswordJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString ForgotPasswordJob::email() const
{
    return mEmail;
}

void ForgotPasswordJob::setEmail(const QString &email)
{
    mEmail = email;
}

QNetworkRequest ForgotPasswordJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ForgotPassword);
    QNetworkRequest request(url);
    addRequestAttribute(request);
    return request;
}

bool ForgotPasswordJob::requireHttpAuthentication() const
{
    return false;
}

QJsonDocument ForgotPasswordJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("email")] = mEmail;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_forgotpasswordjob.cpp"
