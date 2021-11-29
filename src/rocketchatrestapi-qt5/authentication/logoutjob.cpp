/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "logoutjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
LogoutJob::LogoutJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

LogoutJob::~LogoutJob() = default;

bool LogoutJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start LogoutJob job";
        deleteLater();
        return false;
    }

    QNetworkReply *reply = submitGetRequest();
    addStartRestApiInfo("LogoutJob::start");
    connect(reply, &QNetworkReply::finished, this, &LogoutJob::slotLogout);
    return true;
}

void LogoutJob::slotLogout()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("status")].toString() == QLatin1String("success")) {
            addLoggerInfo(QByteArrayLiteral("LogoutJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            qCDebug(ROCKETCHATQTRESTAPI_LOG) << " Logout";
            Q_EMIT logoutDone(); // connected to RestApiConnection::slotLogout
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning("Error during to logout" + replyJson.toJson(QJsonDocument::Indented));
        }

        reply->deleteLater();
    }
    deleteLater();
}

QNetworkRequest LogoutJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Logout);
    QNetworkRequest req(url);
    addAuthRawHeader(req);
    return req;
}

bool LogoutJob::requireHttpAuthentication() const
{
    return true;
}
