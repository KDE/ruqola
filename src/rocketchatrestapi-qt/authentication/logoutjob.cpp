/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

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

    submitGetRequest();
    addStartRestApiInfo("LogoutJob::start");
    return true;
}

void LogoutJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QLatin1String("status")].toString() == QLatin1String("success")) {
        addLoggerInfo(QByteArrayLiteral("LogoutJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << " Logout";
        Q_EMIT logoutDone(); // connected to RestApiConnection::slotLogout
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("Error during to logout" + replyJson.toJson(QJsonDocument::Indented));
    }
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

#include "moc_logoutjob.cpp"
