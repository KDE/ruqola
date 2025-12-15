/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "logoutjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

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
    if (replyObject["status"_L1].toString() == "success"_L1) {
        addLoggerInfo("LogoutJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
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
