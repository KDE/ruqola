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

#include "usersupdateownbasicinfo.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
UsersUpdateOwnBasicInfo::UsersUpdateOwnBasicInfo(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UsersUpdateOwnBasicInfo::~UsersUpdateOwnBasicInfo()
{
}

bool UsersUpdateOwnBasicInfo::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("UsersUpdateOwnBasicInfo::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &UsersUpdateOwnBasicInfo::slotUpdateOwnBasicInfo);
    return true;
}

void UsersUpdateOwnBasicInfo::slotUpdateOwnBasicInfo()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("UsersUpdateOwnBasicInfo: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT updateOwnBasicInfoDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("UsersUpdateOwnBasicInfo: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool UsersUpdateOwnBasicInfo::requireHttpAuthentication() const
{
    return true;
}

bool UsersUpdateOwnBasicInfo::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
//    if (mStatusUserId.isEmpty()) {
//        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "UsersUpdateOwnBasicInfo: mUserId is empty";
//        return false;
//    }
//    if (mStatus == UsersUpdateOwnBasicInfo::Unknown) {
//        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "UsersUpdateOwnBasicInfo: mStatus is not defined";
//        return false;
//    }
    return true;
}

QNetworkRequest UsersUpdateOwnBasicInfo::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersUpdateOwnBasicInfo);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument UsersUpdateOwnBasicInfo::json() const
{
    QJsonObject jsonObj;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
