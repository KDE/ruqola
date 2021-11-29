/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "user2fadisableemailjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
User2FADisableEmailJob::User2FADisableEmailJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

User2FADisableEmailJob::~User2FADisableEmailJob() = default;

bool User2FADisableEmailJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("User2FADisableEmailJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &User2FADisableEmailJob::slotDisableEmail);
    return true;
}

void User2FADisableEmailJob::slotDisableEmail()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("User2FADisableEmailJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT disableEmailDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("User2FADisableEmailJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool User2FADisableEmailJob::requireHttpAuthentication() const
{
    return true;
}

bool User2FADisableEmailJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QNetworkRequest User2FADisableEmailJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Users2FADisableEmail);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument User2FADisableEmailJob::json() const
{
    QJsonObject jsonObj;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString User2FADisableEmailJob::errorMessage(const QString &str, const QJsonObject &detail)
{
    if (str == QLatin1String("totp-required")) {
        return i18n("TOTP Required");
    }
    return RestApiAbstractJob::errorMessage(str, detail);
}
