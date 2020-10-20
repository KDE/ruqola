/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "customuserstatusupdatejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
CustomUserStatusUpdateJob::CustomUserStatusUpdateJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

CustomUserStatusUpdateJob::~CustomUserStatusUpdateJob()
{
}

bool CustomUserStatusUpdateJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("CustomUserStatusUpdateJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &CustomUserStatusUpdateJob::slotCustomUserUpdate);
    return true;
}

void CustomUserStatusUpdateJob::slotCustomUserUpdate()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("CustomUserStatusUpdateJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT customUserUpdateDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("CustomUserStatusUpdateJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool CustomUserStatusUpdateJob::requireHttpAuthentication() const
{
    return true;
}

bool CustomUserStatusUpdateJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
//    if (!mCreateInfo.isValid()) {
//        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "CustomUserStatusUpdateJob: mCreateInfo is not valid.";
//        return false;
//    }
    return true;
}

QNetworkRequest CustomUserStatusUpdateJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::CustomUserStatusUpdate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument CustomUserStatusUpdateJob::json() const
{
    QJsonObject jsonObj;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
