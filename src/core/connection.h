/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "channels/channelhistoryjob.h"
#include "config-ruqola.h"
#include "createchannelteaminfo.h"
#include "libruqolacore_export.h"
#include "users/registeruserjob.h"
#include "users/setavatarjob.h"
#include "users/setstatusjob.h"
#include "users/userbasejob.h"
#include "users/usersautocompletejob.h"
#include "users/userssetpreferencesjob.h"
#include "utils.h"
#include <QObject>
#include <QSslError>
#include <QUrl>

class QNetworkAccessManager;
class QNetworkReply;
class QNetworkCookieJar;
class RESTAuthenticationManager;
namespace RocketChatRestApi
{
class RestApiAbstractJob;
class DownloadFileJob;
class AbstractLogger;
}

class LIBRUQOLACORE_EXPORT Connection : public QObject
{
    Q_OBJECT
public:
    explicit Connection(QObject *parent = nullptr);
    ~Connection() override;

    void setRestApiLogger(RocketChatRestApi::AbstractLogger *logger);
    [[nodiscard]] RESTAuthenticationManager *authenticationManager() const;

    [[nodiscard]] QString userId() const;
    [[nodiscard]] QString authToken() const;
    void setUserId(const QString &userId);
    void setAuthToken(const QString &authToken);

    // Assign/get server url
    [[nodiscard]] QString serverUrl() const;
    void setServerUrl(const QString &serverUrl);

    void setUserName(const QString &userName);
    void setPassword(const QString &password);

    void initializeRestApiJob(RocketChatRestApi::RestApiAbstractJob *job);

    void logout();

    void serverInfo();
    void getPrivateSettings();
    void getOwnInfo();
    RocketChatRestApi::DownloadFileJob *
    downloadFile(const QUrl &url, const QUrl &localFileUrl, const QByteArray &mimeType = "text/plain", bool requiredAuthentication = true);
    void postMessage(const QByteArray &roomId, const QString &text);
    void createChannels(const RocketChatRestApi::CreateChannelTeamInfo &info);
    void createGroups(const RocketChatRestApi::CreateChannelTeamInfo &info);
    void leaveChannel(const QByteArray &roomId);
    void leaveGroups(const QByteArray &roomId);
    void updateMessage(const QByteArray &roomId, const QByteArray &messageId, const QString &text);
    void reactOnMessage(const QByteArray &messageId, const QString &emoji, bool shouldReact);
    void historyChannel(const QString &roomId, const QString &type);
    void filesInRoom(const QByteArray &roomId, const QString &type, int offset = 0, int count = 50);
    void addUserInChannel(const QByteArray &roomId, const QByteArray &userId);

    void markRoomAsRead(const QByteArray &roomId);
    void markAsFavorite(const QByteArray &roomId, bool favorite);
    void addUserInGroup(const QByteArray &roomId, const QByteArray &userId);
    void disableNotifications(const QByteArray &roomId, bool value);
    void hideUnreadStatus(const QByteArray &roomId, bool value);
    void desktopNotifications(const QByteArray &roomId, const QString &value);
    void emailNotifications(const QByteArray &roomId, const QString &value);
    void mobilePushNotifications(const QByteArray &roomId, const QString &value);
    void unreadAlert(const QByteArray &roomId, const QString &value);
    void setAvatar(const RocketChatRestApi::UserBaseJob::UserInfo &info, const RocketChatRestApi::SetAvatarJob::SetAvatarInfo &avatarInfo);
    void forgotPassword(const QString &email);
    void ignoreUser(const QByteArray &roomId, const QByteArray &userId, bool ignore);
    void getGroupRoles(const QByteArray &roomId);
    void getChannelRoles(const QByteArray &roomId);
    void listAllPermissions();
    void muteGroupMentions(const QByteArray &roomId, bool value);

    void channelKick(const QByteArray &roomId, const QByteArray &userId);
    void groupKick(const QByteArray &roomId, const QByteArray &userId);
    void groupAddModerator(const QByteArray &roomId, const QString &userId);
    void groupRemoveModerator(const QByteArray &roomId, const QString &userId);
    void groupAddLeader(const QByteArray &roomId, const QString &userId);
    void groupRemoveLeader(const QByteArray &roomId, const QString &userId);
    void groupAddOwner(const QByteArray &roomId, const QString &userId);
    void groupRemoveOwner(const QByteArray &roomId, const QString &userId);
    void channelAddModerator(const QByteArray &roomId, const QString &userId);
    void channelRemoveModerator(const QByteArray &roomId, const QString &userId);
    void channelAddOwner(const QByteArray &roomId, const QString &userId);
    void channelRemoveOwner(const QByteArray &roomId, const QString &userId);
    void channelDelete(const QByteArray &roomId);
    void groupDelete(const QByteArray &roomId);
    void membersInRoom(const QByteArray &roomId, const QString &type, int offset = 0, int count = 50);
    void channelAddLeader(const QByteArray &roomId, const QString &userId);
    void channelRemoveLeader(const QByteArray &roomId, const QString &userId);
    void desktopDurationNotifications(const QByteArray &roomId, int value);
    void desktopSoundNotifications(const QByteArray &roomId, const QString &value);

    void getDiscussions(Utils::ListMessagesInfo &&info);
    void getThreadsList(const Utils::ListMessagesInfo &info);
    void getPinnedMessages(const Utils::ListMessagesInfo &info);
    void getStarredMessages(const Utils::ListMessagesInfo &info);
    void getSnippetedMessages(const Utils::ListMessagesInfo &info);
    void getMentionedMessages(const Utils::ListMessagesInfo &info);

    void getThreadMessages(const QByteArray &threadMessageId);
    void sendMessage(const QByteArray &roomId, const QString &text, const QString &messageId = QString(), const QByteArray &threadMessageId = QByteArray());
    void setUserStatus(const QString &userId, RocketChatRestApi::SetStatusJob::StatusType status, const QString &message = QString());
    void usersPresence();
    void usersAutocomplete(const RocketChatRestApi::UsersAutocompleteJob::UsersAutocompleterInfo &info);
    void registerNewUser(const RocketChatRestApi::RegisterUserJob::RegisterUserInfo &userInfo);
    void getChannelsCounter(const QByteArray &roomId);
    void setUserPreferences(const RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo &info);
    void hideMentionStatus(const QByteArray &roomId, bool value);
    void historyChannel(const RocketChatRestApi::ChannelHistoryJob::ChannelHistoryInfo &info);

    void muteUser(const QByteArray &roomId, const QString &userName, bool mute);
    void membersInRoomByRole(const QByteArray &roomId, const QString &filter = {}, int offset = 0, int count = 50);
Q_SIGNALS:
    void downloadFileDone(const QUrl &url, const QUrl &localFileUrl);
    void serverInfoDone(const QString &version);
    void serverInfoFailed(bool useDeprecatedVersion);
    void getOwnInfoDone(const QJsonObject &data);
    void privateInfoDone(const QJsonObject &data);
    void channelFilesDone(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void channelMembersDone(const QJsonObject &obj, const QByteArray &roomIdentifier, const QString &filter);
    void channelRolesDone(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void permissionListAllDone(const QJsonObject &obj);
    void groupRolesDone(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void getDiscussionsDone(const QJsonObject &obj, const QByteArray &roomId);
    void getThreadsDone(const QJsonObject &obj, const QString &roomId, bool onlyUnread);
    void getThreadMessagesDone(const QJsonObject &obj, const QByteArray &threadMessageId);
    void getPinnedMessagesDone(const QJsonObject &obj, const QByteArray &roomId);
    void getStarredMessagesDone(const QJsonObject &obj, const QByteArray &roomId);
    void getSnippetedMessagesDone(const QJsonObject &obj, const QByteArray &roomId);
    void getMentionedMessagesDone(const QJsonObject &obj, const QByteArray &roomId);
    void usersPresenceDone(const QJsonObject &obj);

    void usersAutocompleteDone(const QJsonObject &obj);

    void failed(const QString &str);
    void networkError();

    void runCommandDone();
    void registerUserDone();
    void channelGetCountersDone(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void markAsReadDone(const QByteArray &roomId);
    void usersSetPreferencesDone(const QJsonObject &replyObject);
    void postMessageDone(const QJsonObject &replyObject);
    void updateMessageFailed(const QString &str);
    void forgotPasswordDone();

    void loginStatusChanged();
    void createChannelDone(const QJsonObject &replyObject);
    void createGroupDone(const QJsonObject &replyObject);

private:
    LIBRUQOLACORE_NO_EXPORT void initializeCookies();
    LIBRUQOLACORE_NO_EXPORT void slotResult(QNetworkReply *reply);
    LIBRUQOLACORE_NO_EXPORT void slotSslErrors(QNetworkReply *reply, const QList<QSslError> &error);
    LIBRUQOLACORE_NO_EXPORT void slotLogout();
    LIBRUQOLACORE_NO_EXPORT void slotLogin(const QString &authToken, const QString &userId);
    LIBRUQOLACORE_NO_EXPORT void setJoinCodeChannel(const QString &roomId, const QString &joinCode);

    QNetworkAccessManager *const mNetworkAccessManager;
    QNetworkCookieJar *const mCookieJar;
    RocketChatRestApi::RestApiMethod *const mRestApiMethod;
    RESTAuthenticationManager *const mRESTAuthenticationManager;
    RocketChatRestApi::AbstractLogger *mRuqolaLogger = nullptr;
    QString mUserId;
    QString mAuthToken;
    QString mUserName;
    QString mPassword;
    bool mNetworkErrorEmitted = false;
};
