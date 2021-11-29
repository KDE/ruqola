/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &ForgotPasswordJob::slotForgotPassword);
    return true;
}

void ForgotPasswordJob::slotForgotPassword()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("ForgotPasswordJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT forgotPasswordDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("ForgotPasswordJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
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
