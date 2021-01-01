/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "customuserstatuscreatejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
CustomUserStatusCreateJob::CustomUserStatusCreateJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

CustomUserStatusCreateJob::~CustomUserStatusCreateJob()
{
}

bool CustomUserStatusCreateJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("CustomUserStatusCreateJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &CustomUserStatusCreateJob::slotCreateUserStatus);
    return true;
}

void CustomUserStatusCreateJob::slotCreateUserStatus()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("CustomUserStatusCreateJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT createUserStatusDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("CustomUserStatusCreateJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

CustomUserStatusCreateJob::StatusCreateInfo CustomUserStatusCreateJob::statusCreateInfo() const
{
    return mStatusCreateInfo;
}

void CustomUserStatusCreateJob::setStatusCreateInfo(const StatusCreateInfo &statusCreateInfo)
{
    mStatusCreateInfo = statusCreateInfo;
}

bool CustomUserStatusCreateJob::requireHttpAuthentication() const
{
    return true;
}

bool CustomUserStatusCreateJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mStatusCreateInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "CustomUserStatusCreateJob: mStatusCreateInfo is not valid.";
        return false;
    }
    return true;
}

QNetworkRequest CustomUserStatusCreateJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::CustomUserStatusCreate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument CustomUserStatusCreateJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("name")] = mStatusCreateInfo.name;
    jsonObj[QLatin1String("statusType")] = mStatusCreateInfo.statusType;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
