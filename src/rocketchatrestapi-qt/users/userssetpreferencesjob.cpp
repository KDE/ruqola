/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userssetpreferencesjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <KLocalizedString>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
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
    if (replyObject[QLatin1StringView("success")].toBool()) {
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
    jsonObj[QLatin1StringView("userId")] = QString::fromLatin1(mUsersSetPreferencesInfo.userId);
    if (!mUsersSetPreferencesInfo.newRoomNotification.isEmpty()) {
        dataObj[QLatin1StringView("newRoomNotification")] = mUsersSetPreferencesInfo.newRoomNotification;
    }
    if (!mUsersSetPreferencesInfo.newMessageNotification.isEmpty()) {
        dataObj[QLatin1StringView("newMessageNotification")] = mUsersSetPreferencesInfo.newMessageNotification;
    }
    if (!mUsersSetPreferencesInfo.desktopNotifications.isEmpty()) {
        dataObj[QLatin1StringView("desktopNotifications")] = mUsersSetPreferencesInfo.desktopNotifications;
    }
    if (!mUsersSetPreferencesInfo.pushNotifications.isEmpty()) {
        dataObj[QLatin1StringView("pushNotifications")] = mUsersSetPreferencesInfo.pushNotifications;
    }
    if (!mUsersSetPreferencesInfo.emailNotificationMode.isEmpty()) {
        dataObj[QLatin1StringView("emailNotificationMode")] = mUsersSetPreferencesInfo.emailNotificationMode;
    }
    if (!mUsersSetPreferencesInfo.highlights.isEmpty()) {
        dataObj[QLatin1StringView("highlights")] = QJsonArray::fromStringList(mUsersSetPreferencesInfo.highlights);
    }

    if (mUsersSetPreferencesInfo.useEmoji != UsersSetPreferencesInfo::Unknown) {
        dataObj[QLatin1StringView("useEmojis")] = UsersSetPreferencesInfo::convertToBool(mUsersSetPreferencesInfo.useEmoji);
    }
    if (mUsersSetPreferencesInfo.convertAsciiToEmoji != UsersSetPreferencesInfo::Unknown) {
        dataObj[QLatin1StringView("convertAsciiEmoji")] = UsersSetPreferencesInfo::convertToBool(mUsersSetPreferencesInfo.convertAsciiToEmoji);
    }
    if (mUsersSetPreferencesInfo.hideRoles != UsersSetPreferencesInfo::Unknown) {
        dataObj[QLatin1StringView("hideRoles")] = UsersSetPreferencesInfo::convertToBool(mUsersSetPreferencesInfo.hideRoles);
    }
    if (mUsersSetPreferencesInfo.displayAvatars != UsersSetPreferencesInfo::Unknown) {
        dataObj[QLatin1StringView("displayAvatars")] = UsersSetPreferencesInfo::convertToBool(mUsersSetPreferencesInfo.displayAvatars);
    }
    if (mUsersSetPreferencesInfo.sidebarDisplayAvatar != UsersSetPreferencesInfo::Unknown) {
        dataObj[QLatin1StringView("sidebarDisplayAvatar")] = UsersSetPreferencesInfo::convertToBool(mUsersSetPreferencesInfo.sidebarDisplayAvatar);
    }
    if (mUsersSetPreferencesInfo.sidebarShowUnread != UsersSetPreferencesInfo::Unknown) {
        dataObj[QLatin1StringView("sidebarShowUnread")] = UsersSetPreferencesInfo::convertToBool(mUsersSetPreferencesInfo.sidebarShowUnread);
    }
    if (mUsersSetPreferencesInfo.sidebarShowFavorites != UsersSetPreferencesInfo::Unknown) {
        dataObj[QLatin1StringView("sidebarShowFavorites")] = UsersSetPreferencesInfo::convertToBool(mUsersSetPreferencesInfo.sidebarShowFavorites);
    }
    if (!mUsersSetPreferencesInfo.sidebarSortby.isEmpty()) {
        dataObj[QLatin1StringView("sidebarSortby")] = mUsersSetPreferencesInfo.sidebarSortby;
    }
    if (!mUsersSetPreferencesInfo.sidebarViewMode.isEmpty()) {
        dataObj[QLatin1StringView("sidebarViewMode")] = mUsersSetPreferencesInfo.sidebarViewMode;
    }
    if (mUsersSetPreferencesInfo.receiveLoginDetectionEmail != UsersSetPreferencesInfo::Unknown) {
        dataObj[QLatin1StringView("receiveLoginDetectionEmail")] = UsersSetPreferencesInfo::convertToBool(mUsersSetPreferencesInfo.receiveLoginDetectionEmail);
    }
    if (mUsersSetPreferencesInfo.enableAutoAway != UsersSetPreferencesInfo::Unknown) {
        dataObj[QLatin1StringView("enableAutoAway")] = UsersSetPreferencesInfo::convertToBool(mUsersSetPreferencesInfo.enableAutoAway);
    }
    if (mUsersSetPreferencesInfo.idleTimeLimit != -1) {
        dataObj[QLatin1StringView("idleTimeLimit")] = mUsersSetPreferencesInfo.idleTimeLimit;
    }
    jsonObj[QLatin1StringView("data")] = dataObj;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString UsersSetPreferencesJob::errorMessage(const QString &str, const QJsonObject &details)
{
    if (str == QLatin1StringView("invalid-params")) {
        return i18n("Invalid parameters");
    }
    return RestApiAbstractJob::errorMessage(str, details);
}

QDebug operator<<(QDebug d, const RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo &t)
{
    d.space() << "userId:" << t.userId;
    d.space() << "newRoomNotification:" << t.newRoomNotification;
    d.space() << "newMessageNotification:" << t.newMessageNotification;
    d.space() << "desktopNotifications:" << t.desktopNotifications;
    d.space() << "pushNotifications:" << t.pushNotifications;
    d.space() << "emailNotificationMode:" << t.emailNotificationMode;
    d.space() << "userId:" << t.userId;
    d.space() << " highlights:" << t.highlights;
    d.space() << "useEmoji:" << t.useEmoji;
    d.space() << "convertAsciiToEmoji:" << t.convertAsciiToEmoji;
    d.space() << "hideRoles:" << t.hideRoles;
    d.space() << "displayAvatars:" << t.displayAvatars;
    d.space() << "sidebarShowUnread:" << t.sidebarShowUnread;
    d.space() << "sidebarDisplayAvatar:" << t.sidebarDisplayAvatar;
    d.space() << "sidebarShowFavorites:" << t.sidebarShowFavorites;
    d.space() << "sidebarSortby:" << t.sidebarSortby;
    d.space() << "sidebarViewMode:" << t.sidebarViewMode;
    d.space() << "receiveLoginDetectionEmail:" << t.receiveLoginDetectionEmail;
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
