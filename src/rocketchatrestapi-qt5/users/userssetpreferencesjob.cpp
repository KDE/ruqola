/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userssetpreferencesjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"


#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
UsersSetPreferencesJob::UsersSetPreferencesJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UsersSetPreferencesJob::~UsersSetPreferencesJob() = default;

bool UsersSetPreferencesJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("UsersSetPreferencesJob::start");
    submitPostRequest(json());
    return true;
}

void UsersSetPreferencesJob::onPostRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("UsersSetPreferencesJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT usersSetPreferencesDone(replyObject);
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("UsersSetPreferencesJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
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
    dataObj[QLatin1String("hideAvatars")] = mUsersSetPreferencesInfo.hideAvatars;
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
    d << "hideAvatars: " << t.hideAvatars;
    return d;
}

bool UsersSetPreferencesJob::UsersSetPreferencesInfo::isValid() const
{
    return !userId.isEmpty();
}
