/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userssetpreferencesjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <KLocalizedString>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
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
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("UsersSetPreferencesJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT usersSetPreferencesDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("UsersSetPreferencesJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
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
    jsonObj["userId"_L1] = QLatin1StringView(mUsersSetPreferencesInfo.userId);
    if (!mUsersSetPreferencesInfo.newRoomNotification.isEmpty()) {
        dataObj["newRoomNotification"_L1] = QString::fromLatin1(mUsersSetPreferencesInfo.newRoomNotification);
    }
    if (!mUsersSetPreferencesInfo.newMessageNotification.isEmpty()) {
        dataObj["newMessageNotification"_L1] = QString::fromLatin1(mUsersSetPreferencesInfo.newMessageNotification);
    }
    if (!mUsersSetPreferencesInfo.desktopNotifications.isEmpty()) {
        dataObj["desktopNotifications"_L1] = mUsersSetPreferencesInfo.desktopNotifications;
    }
    if (!mUsersSetPreferencesInfo.pushNotifications.isEmpty()) {
        dataObj["pushNotifications"_L1] = mUsersSetPreferencesInfo.pushNotifications;
    }
    if (!mUsersSetPreferencesInfo.emailNotificationMode.isEmpty()) {
        dataObj["emailNotificationMode"_L1] = mUsersSetPreferencesInfo.emailNotificationMode;
    }
    if (!mUsersSetPreferencesInfo.highlights.isEmpty()) {
        dataObj["highlights"_L1] = QJsonArray::fromStringList(mUsersSetPreferencesInfo.highlights);
    }

    if (mUsersSetPreferencesInfo.useEmoji != UsersSetPreferencesInfo::State::Unknown) {
        dataObj["useEmojis"_L1] = UsersSetPreferencesInfo::convertToBool(mUsersSetPreferencesInfo.useEmoji);
    }
    if (mUsersSetPreferencesInfo.convertAsciiToEmoji != UsersSetPreferencesInfo::State::Unknown) {
        dataObj["convertAsciiEmoji"_L1] = UsersSetPreferencesInfo::convertToBool(mUsersSetPreferencesInfo.convertAsciiToEmoji);
    }
    if (mUsersSetPreferencesInfo.hideRoles != UsersSetPreferencesInfo::State::Unknown) {
        dataObj["hideRoles"_L1] = UsersSetPreferencesInfo::convertToBool(mUsersSetPreferencesInfo.hideRoles);
    }
    if (mUsersSetPreferencesInfo.displayAvatars != UsersSetPreferencesInfo::State::Unknown) {
        dataObj["displayAvatars"_L1] = UsersSetPreferencesInfo::convertToBool(mUsersSetPreferencesInfo.displayAvatars);
    }
    if (mUsersSetPreferencesInfo.sidebarDisplayAvatar != UsersSetPreferencesInfo::State::Unknown) {
        dataObj["sidebarDisplayAvatar"_L1] = UsersSetPreferencesInfo::convertToBool(mUsersSetPreferencesInfo.sidebarDisplayAvatar);
    }
    if (mUsersSetPreferencesInfo.sidebarShowUnread != UsersSetPreferencesInfo::State::Unknown) {
        dataObj["sidebarShowUnread"_L1] = UsersSetPreferencesInfo::convertToBool(mUsersSetPreferencesInfo.sidebarShowUnread);
    }
    if (mUsersSetPreferencesInfo.sidebarShowFavorites != UsersSetPreferencesInfo::State::Unknown) {
        dataObj["sidebarShowFavorites"_L1] = UsersSetPreferencesInfo::convertToBool(mUsersSetPreferencesInfo.sidebarShowFavorites);
    }
    if (!mUsersSetPreferencesInfo.sidebarSortby.isEmpty()) {
        dataObj["sidebarSortby"_L1] = mUsersSetPreferencesInfo.sidebarSortby;
    }
    if (!mUsersSetPreferencesInfo.sidebarViewMode.isEmpty()) {
        dataObj["sidebarViewMode"_L1] = mUsersSetPreferencesInfo.sidebarViewMode;
    }
    if (mUsersSetPreferencesInfo.receiveLoginDetectionEmail != UsersSetPreferencesInfo::State::Unknown) {
        dataObj["receiveLoginDetectionEmail"_L1] = UsersSetPreferencesInfo::convertToBool(mUsersSetPreferencesInfo.receiveLoginDetectionEmail);
    }
    if (mUsersSetPreferencesInfo.enableAutoAway != UsersSetPreferencesInfo::State::Unknown) {
        dataObj["enableAutoAway"_L1] = UsersSetPreferencesInfo::convertToBool(mUsersSetPreferencesInfo.enableAutoAway);
    }
    if (mUsersSetPreferencesInfo.idleTimeLimit != -1) {
        dataObj["idleTimeLimit"_L1] = mUsersSetPreferencesInfo.idleTimeLimit;
    }
    if (mUsersSetPreferencesInfo.muteFocusedConversations != UsersSetPreferencesInfo::State::Unknown) {
        dataObj["muteFocusedConversations"_L1] = UsersSetPreferencesInfo::convertToBool(mUsersSetPreferencesInfo.muteFocusedConversations);
    }
    if (mUsersSetPreferencesInfo.notificationsSoundVolume != -1) {
        dataObj["notificationsSoundVolume"_L1] = mUsersSetPreferencesInfo.notificationsSoundVolume;
    }

    jsonObj["data"_L1] = dataObj;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString UsersSetPreferencesJob::errorMessage(const QString &str, const QJsonObject &details)
{
    if (str == "invalid-params"_L1) {
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
    d.space() << "highlights:" << t.highlights;
    d.space() << "useEmoji:" << t.useEmoji;
    d.space() << "convertAsciiToEmoji:" << t.convertAsciiToEmoji;
    d.space() << "hideRoles:" << t.hideRoles;
    d.space() << "displayAvatars:" << t.displayAvatars;
    d.space() << "sidebarShowUnread:" << t.sidebarShowUnread;
    d.space() << "sidebarDisplayAvatar:" << t.sidebarDisplayAvatar;
    d.space() << "sidebarShowFavorites:" << t.sidebarShowFavorites;
    d.space() << "sidebarSortby:" << t.sidebarSortby;
    d.space() << "sidebarViewMode:" << t.sidebarViewMode;
    d.space() << "idleTimeLimit:" << t.idleTimeLimit;
    d.space() << "receiveLoginDetectionEmail:" << t.receiveLoginDetectionEmail;
    d.space() << "notificationsSoundVolume:" << t.notificationsSoundVolume;
    d.space() << "muteFocusedConversations:" << t.muteFocusedConversations;
    return d;
}

bool UsersSetPreferencesJob::UsersSetPreferencesInfo::isValid() const
{
    return !userId.isEmpty();
}

bool UsersSetPreferencesJob::UsersSetPreferencesInfo::convertToBool(State state)
{
    switch (state) {
    case State::Unknown:
        return false;
    case State::Checked:
        return true;
    case State::Unchecked:
        return false;
    }
    return false;
}

UsersSetPreferencesJob::UsersSetPreferencesInfo::State UsersSetPreferencesJob::UsersSetPreferencesInfo::convertToState(bool checked)
{
    return checked ? UsersSetPreferencesJob::UsersSetPreferencesInfo::State::Checked : UsersSetPreferencesJob::UsersSetPreferencesInfo::State::Unchecked;
}

#include "moc_userssetpreferencesjob.cpp"
