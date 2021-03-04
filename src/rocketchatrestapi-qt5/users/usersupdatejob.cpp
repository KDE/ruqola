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

#include "usersupdatejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
UsersUpdateJob::UsersUpdateJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UsersUpdateJob::~UsersUpdateJob()
{
}

bool UsersUpdateJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("UsersUpdateJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &UsersUpdateJob::slotUsersUpdate);
    return true;
}

void UsersUpdateJob::slotUsersUpdate()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("UsersUpdateJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT usersUpdateDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("UsersUpdateJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

UsersUpdateJob::UpdateInfo UsersUpdateJob::updateInfo() const
{
    return mUpdateInfo;
}

void UsersUpdateJob::setUpdateInfo(const UpdateInfo &updateInfo)
{
    mUpdateInfo = updateInfo;
}

bool UsersUpdateJob::requireHttpAuthentication() const
{
    return true;
}

bool UsersUpdateJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mUpdateInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "UsersUpdateJob: mUpdateInfo is not valid.";
        return false;
    }
    return true;
}

QNetworkRequest UsersUpdateJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersUpdate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#if 0
QJsonDocument UsersSetPreferencesJob::json() const
{
    QJsonObject jsonObj;
    QJsonObject dataObj;
    jsonObj[QLatin1String("userId")] = mUsersSetPreferencesInfo.userId;
    if (!mUsersSetPreferencesInfo.newRoomNotification.isEmpty()) {
        dataObj[QLatin1String("newRoomNotification")] = mUsersSetPreferencesInfo.newRoomNotification;
    }
    if (!mUsersSetPreferencesInfo.newMessageNotification.isEmpty()) {
        dataObj[QLatin1String("newMessageNotification")] = mUsersSetPreferencesInfo.newMessageNotification;
    }
    if (!mUsersSetPreferencesInfo.desktopNotifications.isEmpty()) {
        dataObj[QLatin1String("desktopNotifications")] = mUsersSetPreferencesInfo.desktopNotifications;
    }
    if (!mUsersSetPreferencesInfo.mobileNotifications.isEmpty()) {
        dataObj[QLatin1String("mobileNotifications")] = mUsersSetPreferencesInfo.mobileNotifications;
    }
    if (!mUsersSetPreferencesInfo.emailNotificationMode.isEmpty()) {
        dataObj[QLatin1String("emailNotificationMode")] = mUsersSetPreferencesInfo.emailNotificationMode;
    }

    dataObj[QLatin1String("useEmojis")] = mUsersSetPreferencesInfo.useEmoji;
    dataObj[QLatin1String("convertAsciiEmoji")] = mUsersSetPreferencesInfo.convertAsciiToEmoji;
    dataObj[QLatin1String("highlights")] = QJsonArray::fromStringList(mUsersSetPreferencesInfo.highlights);
    dataObj[QLatin1String("hideRoles")] = mUsersSetPreferencesInfo.hideRoles;
    dataObj[QLatin1String("hideAvatars")] = mUsersSetPreferencesInfo.hideAvatars;
    jsonObj[QLatin1String("data")] = dataObj;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#endif

QJsonDocument UsersUpdateJob::json() const
{
    QJsonObject jsonObj;
    QJsonObject dataObj;
    dataObj[QLatin1String("email")] = mUpdateInfo.mEmail;
    dataObj[QLatin1String("name")] = mUpdateInfo.mName;
    dataObj[QLatin1String("password")] = mUpdateInfo.mPassword;
    dataObj[QLatin1String("username")] = mUpdateInfo.mUserName;

    jsonObj[QLatin1String("data")] = dataObj;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool UsersUpdateJob::UpdateInfo::isValid() const
{
    return !mUserId.isEmpty();
}

QDebug operator<<(QDebug d, const RocketChatRestApi::UsersUpdateJob::UpdateInfo &t)
{
    d << "userId : " << t.mUserId;
    d << "mEmail : " << t.mEmail;
    d << "mName : " << t.mName;
    d << "mPassword : " << t.mPassword;
    d << "mUserName : " << t.mUserName;
    return d;
}
