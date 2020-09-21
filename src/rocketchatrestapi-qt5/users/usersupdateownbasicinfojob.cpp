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

#include "usersupdateownbasicinfojob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <KLocalizedString>

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
UsersUpdateOwnBasicInfoJob::UsersUpdateOwnBasicInfoJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UsersUpdateOwnBasicInfoJob::~UsersUpdateOwnBasicInfoJob()
{
}

bool UsersUpdateOwnBasicInfoJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("UsersUpdateOwnBasicInfo::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &UsersUpdateOwnBasicInfoJob::slotUpdateOwnBasicInfo);
    return true;
}

void UsersUpdateOwnBasicInfoJob::slotUpdateOwnBasicInfo()
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

UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo UsersUpdateOwnBasicInfoJob::updateOwnBasicInfo() const
{
    return mUpdateOwnBasicInfo;
}

void UsersUpdateOwnBasicInfoJob::setUpdateOwnBasicInfo(const UpdateOwnBasicInfo &updateOwnBasicInfo)
{
    mUpdateOwnBasicInfo = updateOwnBasicInfo;
}

QString UsersUpdateOwnBasicInfoJob::errorMessage(const QString &str)
{
    if (str == QLatin1String("totp-required")) {
        return i18n("Two Authentication Password Required");
    }
    return RestApiAbstractJob::errorMessage(str);
}

bool UsersUpdateOwnBasicInfoJob::requireHttpAuthentication() const
{
    return true;
}

bool UsersUpdateOwnBasicInfoJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mUpdateOwnBasicInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "UsersUpdateOwnBasicInfo: mUpdateOwnBasicInfo is not valid.";
        return false;
    }
    return true;
}

QNetworkRequest UsersUpdateOwnBasicInfoJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersUpdateOwnBasicInfo);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument UsersUpdateOwnBasicInfoJob::json() const
{
    QJsonObject jsonObj;
    QJsonObject dataObj;
    if (!mUpdateOwnBasicInfo.email.isEmpty()) {
        dataObj[QLatin1String("email")] = mUpdateOwnBasicInfo.email;
    }
    if (!mUpdateOwnBasicInfo.userName.isEmpty()) {
        dataObj[QLatin1String("username")] = mUpdateOwnBasicInfo.userName;
    }
    if (!mUpdateOwnBasicInfo.nickName.isEmpty()) {
        dataObj[QLatin1String("nickname")] = mUpdateOwnBasicInfo.nickName;
    }
    if (!mUpdateOwnBasicInfo.statusText.isEmpty()) {
        dataObj[QLatin1String("statustext")] = mUpdateOwnBasicInfo.statusText;
    }
    if (!mUpdateOwnBasicInfo.newPassword.isEmpty()) {
        dataObj[QLatin1String("currentPassword")] = mUpdateOwnBasicInfo.currentPassword;
        dataObj[QLatin1String("newPassword")] = mUpdateOwnBasicInfo.newPassword;
        //TODO use ssh256
    }

    jsonObj[QLatin1String("data")] = dataObj;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo::isValid() const
{
    return !email.isEmpty()
           || !userName.isEmpty()
           || !nickName.isEmpty()
           || !statusText.isEmpty()
           || (!newPassword.isEmpty() && !currentPassword.isEmpty());
}
