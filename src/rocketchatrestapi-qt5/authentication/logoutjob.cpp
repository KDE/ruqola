/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "logoutjob.h"
#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
LogoutJob::LogoutJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

LogoutJob::~LogoutJob()
{
}

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
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        addLoggerInfo("LogoutJob finished: " + data);
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("status")].toString() == QLatin1String("success")) {
            qCDebug(ROCKETCHATQTRESTAPI_LOG) << " Logout";
            Q_EMIT logoutDone(); // connected to RestApiRequest::slotLogout
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning("Error during to logout" + data);
        }

        qCDebug(ROCKETCHATQTRESTAPI_LOG) << " void RestApiRequest::parseLogout(const QByteArray &data)" << data;
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
