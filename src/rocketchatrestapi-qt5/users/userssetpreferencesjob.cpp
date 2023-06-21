/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

void UsersSetPreferencesJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("UsersSetPreferencesJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT usersSetPreferencesDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
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
    if (!mUsersSetPreferencesInfo.pushNotifications.isEmpty()) {
        dataObj[QLatin1String("pushNotifications")] = mUsersSetPreferencesInfo.pushNotifications;
    }
    if (!mUsersSetPreferencesInfo.emailNotificationMode.isEmpty()) {
        dataObj[QLatin1String("emailNotificationMode")] = mUsersSetPreferencesInfo.emailNotificationMode;
    }
    if (!mUsersSetPreferencesInfo.highlights.isEmpty()) {
        dataObj[QLatin1String("highlights")] = QJsonArray::fromStringList(mUsersSetPreferencesInfo.highlights);
    }

    if (mUsersSetPreferencesInfo.useEmoji != UsersSetPreferencesInfo::Unknown) {
        dataObj[QLatin1String("useEmojis")] = UsersSetPreferencesInfo::convertToBool(mUsersSetPreferencesInfo.useEmoji);
    }
    if (mUsersSetPreferencesInfo.convertAsciiToEmoji != UsersSetPreferencesInfo::Unknown) {
        dataObj[QLatin1String("convertAsciiEmoji")] = UsersSetPreferencesInfo::convertToBool(mUsersSetPreferencesInfo.convertAsciiToEmoji);
    }
    if (mUsersSetPreferencesInfo.hideRoles != UsersSetPreferencesInfo::Unknown) {
        dataObj[QLatin1String("hideRoles")] = UsersSetPreferencesInfo::convertToBool(mUsersSetPreferencesInfo.hideRoles);
    }
    if (mUsersSetPreferencesInfo.displayAvatars != UsersSetPreferencesInfo::Unknown) {
        dataObj[QLatin1String("displayAvatars")] = UsersSetPreferencesInfo::convertToBool(mUsersSetPreferencesInfo.displayAvatars);
    }
    if (mUsersSetPreferencesInfo.sidebarDisplayAvatar != UsersSetPreferencesInfo::Unknown) {
        dataObj[QLatin1String("sidebarDisplayAvatar")] = UsersSetPreferencesInfo::convertToBool(mUsersSetPreferencesInfo.sidebarDisplayAvatar);
    }
    if (mUsersSetPreferencesInfo.sidebarShowUnread != UsersSetPreferencesInfo::Unknown) {
        dataObj[QLatin1String("sidebarShowUnread")] = UsersSetPreferencesInfo::convertToBool(mUsersSetPreferencesInfo.sidebarShowUnread);
    }
    if (mUsersSetPreferencesInfo.sidebarShowFavorites != UsersSetPreferencesInfo::Unknown) {
        dataObj[QLatin1String("sidebarShowFavorites")] = UsersSetPreferencesInfo::convertToBool(mUsersSetPreferencesInfo.sidebarShowFavorites);
    }
    if (!mUsersSetPreferencesInfo.sidebarSortby.isEmpty()) {
        dataObj[QLatin1String("sidebarSortby")] = mUsersSetPreferencesInfo.sidebarSortby;
    }
    if (mUsersSetPreferencesInfo.receiveLoginDetectionEmail != UsersSetPreferencesInfo::Unknown) {
        dataObj[QLatin1String("receiveLoginDetectionEmail")] = UsersSetPreferencesInfo::convertToBool(mUsersSetPreferencesInfo.receiveLoginDetectionEmail);
    }
    jsonObj[QLatin1String("data")] = dataObj;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString UsersSetPreferencesJob::errorMessage(const QString &str, const QJsonObject &details)
{
    if (str == QLatin1String("invalid-params")) {
        return i18n("Invalid parameters");
    }
    return RestApiAbstractJob::errorMessage(str, details);
}

QDebug operator<<(QDebug d, const RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo &t)
{
    d << "userId : " << t.userId;
    d << "newRoomNotification : " << t.newRoomNotification;
    d << "newMessageNotification : " << t.newMessageNotification;
    d << "desktopNotifications : " << t.desktopNotifications;
    d << "pushNotifications : " << t.pushNotifications;
    d << "emailNotificationMode: " << t.emailNotificationMode;
    d << "userId : " << t.userId;
    d << " highlights : " << t.highlights;
    d << "useEmoji: " << t.useEmoji;
    d << "convertAsciiToEmoji: " << t.convertAsciiToEmoji;
    d << "hideRoles: " << t.hideRoles;
    d << "displayAvatars: " << t.displayAvatars;
    d << "sidebarShowUnread: " << t.sidebarShowUnread;
    d << "sidebarDisplayAvatar " << t.sidebarDisplayAvatar;
    d << "sidebarShowFavorites " << t.sidebarShowFavorites;
    d << "sidebarSortby: " << t.sidebarSortby;
    d << "receiveLoginDetectionEmail: " << t.receiveLoginDetectionEmail;
    return d;
}

bool UsersSetPreferencesJob::UsersSetPreferencesInfo::isValid() const
{
    return !userId.isEmpty();
}

bool UsersSetPreferencesJob::UsersSetPreferencesInfo::convertToBool(State state)
{
    switch (state) {
    case Unknown:
        return false;
    case Checked:
        return true;
    case Unchecked:
        return false;
    }
    return false;
}

UsersSetPreferencesJob::UsersSetPreferencesInfo::State UsersSetPreferencesJob::UsersSetPreferencesInfo::convertToState(bool checked)
{
    return checked ? UsersSetPreferencesJob::UsersSetPreferencesInfo::Checked : UsersSetPreferencesJob::UsersSetPreferencesInfo::Unchecked;
}

#include "moc_userssetpreferencesjob.cpp"
