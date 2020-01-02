/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

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

#include "customuserstatusjob.h"
#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
CustomUserStatusJob::CustomUserStatusJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

CustomUserStatusJob::~CustomUserStatusJob()
{
}

bool CustomUserStatusJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start CustomUserStatusJob job";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = mNetworkAccessManager->get(request());
    addLoggerInfo(QByteArrayLiteral("CustomUserStatusJob: Ask custom user status info"));
    connect(reply, &QNetworkReply::finished, this, &CustomUserStatusJob::slotCustomUserStatusDone);

    return true;
}

void CustomUserStatusJob::slotCustomUserStatusDone()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("CustomUserStatusJob done: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT customUserStatusDone(data);
        } else {
            emitFailedMessage(replyObject);
            addLoggerWarning(QByteArrayLiteral("CustomUserStatusJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool CustomUserStatusJob::requireHttpAuthentication() const
{
    return true;
}

QNetworkRequest CustomUserStatusJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::CustomUserStatusList);
    QNetworkRequest req(url);
    addAuthRawHeader(req);
    return req;
}
