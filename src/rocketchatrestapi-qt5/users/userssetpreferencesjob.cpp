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

#include "userssetpreferencesjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <KLocalizedString>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
UsersSetPreferencesJob::UsersSetPreferencesJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UsersSetPreferencesJob::~UsersSetPreferencesJob()
{
}

bool UsersSetPreferencesJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("UsersSetPreferencesJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &UsersSetPreferencesJob::slotUsersSetPreferences);
    return true;
}

void UsersSetPreferencesJob::slotUsersSetPreferences()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("UsersSetPreferencesJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT usersSetPreferencesDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("UsersSetPreferencesJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

UsersSetPreferencesJob::UsersSetPreferencesInfo UsersSetPreferencesJob::usersSetPreferencesInfo() const
{
    return mUsersSetPreferencesInfo;
}

void UsersSetPreferencesJob::setUsersSetPreferencesInfo(const UsersSetPreferencesInfo &usersSetPreferencesInfo)
{
    mUsersSetPreferencesInfo = usersSetPreferencesInfo;
}

bool UsersSetPreferencesJob::requireHttpAuthentication() const
{
    return true;
}

bool UsersSetPreferencesJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mUsersSetPreferencesInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "UsersSetPreferencesJob: mUsersSetPreferencesInfo is not valid.";
        return false;
    }
    return true;
}

QNetworkRequest UsersSetPreferencesJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersSetPreferences);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

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
    jsonObj[QLatin1String("data")] = dataObj;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QDebug operator<<(QDebug d, const RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo &t)
{
    d << "userId : " << t.userId;
    d << "newRoomNotification : " << t.newRoomNotification;
    d << "newMessageNotification : " << t.newMessageNotification;
    d << "desktopNotifications : " << t.desktopNotifications;
    d << "mobileNotifications : " << t.mobileNotifications;
    d << "emailNotificationMode: " << t.emailNotificationMode;
    d << "userId : " << t.userId;
    d << " highlights : " << t.highlights;
    d << "useEmoji: " << t.useEmoji;
    d << "convertAsciiToEmoji: " << t.convertAsciiToEmoji;
    d << "hideRoles: " << t.hideRoles;
    return d;
}

bool UsersSetPreferencesJob::UsersSetPreferencesInfo::isValid() const
{
    return !userId.isEmpty();
}
