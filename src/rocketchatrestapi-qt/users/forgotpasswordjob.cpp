/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "forgotpasswordjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
ForgotPasswordJob::ForgotPasswordJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ForgotPasswordJob::~ForgotPasswordJob() = default;

bool ForgotPasswordJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mEmail.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Email is empty";
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
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("ForgotPasswordJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT forgotPasswordDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("ForgotPasswordJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
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
    jsonObj["email"_L1] = mEmail;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_forgotpasswordjob.cpp"
