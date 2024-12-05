/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "channels/channelhistoryjob.h"
#include "commands/runcommandjob.h"
#include "config-ruqola.h"
#include "createchannelteaminfo.h"
#include "custom/customuserstatuscreatejob.h"
#include "custom/customuserstatusupdatejob.h"
#include "invite/findorcreateinvitejob.h"
#include "libruqolacore_export.h"
#include "rooms/adminroomsjob.h"
#include "rooms/roomscleanhistoryjob.h"
#include "users/registeruserjob.h"
#include "users/setavatarjob.h"
#include "users/setstatusjob.h"
#include "users/userbasejob.h"
#include "users/usersautocompletejob.h"
#include "users/userssetpreferencesjob.h"
#include "users/usersupdateownbasicinfojob.h"
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

    void login();
    void logout();
    void getAvatar(const RocketChatRestApi::UserBaseJob::UserInfo &info);

    void serverInfo();
    void getPrivateSettings();
    void getOwnInfo();
    void starMessage(const QByteArray &messageId, bool starred);
    RocketChatRestApi::DownloadFileJob *
    downloadFile(const QUrl &url, const QUrl &localFileUrl, const QByteArray &mimeType = "text/plain", bool requiredAuthentication = true);
    void changeChannelTopic(const QString &roomId, const QString &topic);
    void changeGroupsTopic(const QString &roomId, const QString &topic);
    void changeChannelAnnouncement(const QString &roomId, const QString &announcement);
    void changeGroupsAnnouncement(const QString &roomId, const QString &announcement);
    void postMessage(const QByteArray &roomId, const QString &text);
    void deleteMessage(const QByteArray &roomId, const QByteArray &messageId);
    void createChannels(const RocketChatRestApi::CreateChannelTeamInfo &info);
    void createGroups(const RocketChatRestApi::CreateChannelTeamInfo &info);
    void leaveChannel(const QByteArray &roomId);
    void leaveGroups(const QByteArray &roomId);
    void updateMessage(const QByteArray &roomId, const QByteArray &messageId, const QString &text);
    void reactOnMessage(const QByteArray &messageId, const QString &emoji, bool shouldReact);
    void closeChannel(const QByteArray &roomId, const QString &type);
    void createDirectMessage(const QStringList &userName);
    void historyChannel(const QString &roomId, const QString &type);
    void changeChannelDescription(const QString &roomId, const QString &description);
    void changeGroupsDescription(const QString &roomId, const QString &description);
    void archiveChannel(const QString &roomId, bool archive);
    void filesInRoom(const QByteArray &roomId, const QString &type, int offset = 0, int count = 50);
    void addUserInChannel(const QByteArray &roomId, const QByteArray &userId);

    void markRoomAsRead(const QByteArray &roomId);
    void getRooms();
    void markAsFavorite(const QByteArray &roomId, bool favorite);
    void addUserInGroup(const QByteArray &roomId, const QByteArray &userId);
    void disableNotifications(const QByteArray &roomId, bool value);
    void hideUnreadStatus(const QByteArray &roomId, bool value);
    void desktopNotifications(const QByteArray &roomId, const QString &value);
    void emailNotifications(const QByteArray &roomId, const QString &value);
    void mobilePushNotifications(const QByteArray &roomId, const QString &value);
    void unreadAlert(const QByteArray &roomId, const QString &value);
    void setAvatar(const RocketChatRestApi::UserBaseJob::UserInfo &info, const RocketChatRestApi::SetAvatarJob::SetAvatarInfo &avatarInfo);
    void markRoomAsUnRead(const QByteArray &roomId);
    void markMessageAsUnReadFrom(const QByteArray &messageId);
    void forgotPassword(const QString &email);
    void userInfo(const QString &identifier, bool userName = false);
    void ignoreUser(const QByteArray &roomId, const QByteArray &userId, bool ignore);
    void userPresence(const QString &userId);
    void reportMessage(const QByteArray &messageId, const QString &message);
    void setGroupType(const QString &roomId, bool isPrivate);
    void setChannelType(const QString &roomId, bool isPrivate);
    void getGroupRoles(const QByteArray &roomId);
    void getChannelRoles(const QByteArray &roomId);
    void getUsernameSuggestion();
    void listAllPermissions();
    void setJoinCodeChannel(const QString &roomId, const QString &joinCode);
    void muteGroupMentions(const QByteArray &roomId, bool value);

    void channelInfo(const QByteArray &roomId);

    void syncThreadMessages(const QString &threadMessageId, const QString &timestamp);
    void changeChannelName(const QString &roomId, const QString &newName);
    void changeGroupName(const QString &roomId, const QString &newName);
    void groupInfo(const QByteArray &roomId);
    void channelGetAllUserMentions(const QString &roomId, int offset = 0, int count = 50);
    void openDirectMessage(const QString &userId);
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
    void changeChannelReadOnly(const QString &roomId, bool b);
    void changeGroupsReadOnly(const QString &roomId, bool b);
    void channelAddLeader(const QByteArray &roomId, const QString &userId);
    void channelRemoveLeader(const QByteArray &roomId, const QString &userId);
    void changeGroupsEncrypted(const QString &roomId, bool b);
    void pinMessage(const QByteArray &messageId, bool pinned);
    void desktopDurationNotifications(const QByteArray &roomId, int value);
    void desktopSoundNotifications(const QByteArray &roomId, const QString &value);
    void followMessage(const QByteArray &messageId);
    void unFollowMessage(const QByteArray &messageId);
    void createDiscussion(const QByteArray &parentRoomId,
                          const QString &discussionName,
                          const QString &replyMessage,
                          const QByteArray &parentMessageId,
                          const QList<QByteArray> &users);
    void getDiscussions(const QByteArray &roomId, int offset = 0, int count = 50);
    void getThreadsList(const QByteArray &roomId, bool onlyUnread, int offset = 0, int count = 50, bool useSyntaxRc70 = false);
    void getThreadMessages(const QByteArray &threadMessageId);
    void sendMessage(const QByteArray &roomId, const QString &text, const QString &messageId = QString(), const QByteArray &threadMessageId = QByteArray());
    void getPinnedMessages(const QByteArray &roomId, int offset = 0, int count = 50);
    void autoTranslateSaveLanguageSettings(const QByteArray &roomId, const QString &language);
    void autoTranslateSaveAutoTranslateSettings(const QByteArray &roomId, bool autoTranslate);
    void setUserStatus(const QString &userId, RocketChatRestApi::SetStatusJob::StatusType status, const QString &message = QString());
    void usersPresence();
    void getStarredMessages(const QByteArray &roomId, int offset = 0, int count = 50);
    void getSnippetedMessages(const QByteArray &roomId, int offset = 0, int count = 50);
    void customUserStatus();
    void usersAutocomplete(const RocketChatRestApi::UsersAutocompleteJob::UsersAutocompleterInfo &info);
    void findOrCreateInvite(const QByteArray &roomId, int maxUses, int numberOfDays);
    void getMessage(const QByteArray &messageId, const QByteArray &roomId);
    void getCommand(const QString &commandName);
    void runCommand(const RocketChatRestApi::RunCommandJob::RunCommandInfo &runCommandInfo);
    void sendUserEmailCode(const QString &identifier);
    void deleteOwnAccount(const QString &password);
    void registerNewUser(const RocketChatRestApi::RegisterUserJob::RegisterUserInfo &userInfo);
    void enable2FaEmailJob(bool enable);
    void updateOwnBasicInfo(const RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo &info);
    void cleanChannelHistory(const RocketChatRestApi::RoomsCleanHistoryJob::CleanHistoryInfo &info);
    void getRoomsAdmin(const RocketChatRestApi::AdminRoomsJob::AdminRoomsJobInfo &info);
    void getChannelsCounter(const QByteArray &roomId);
    void setUserPreferences(const RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo &info);
    void createCustomUserStatus(const RocketChatRestApi::CustomUserStatusCreateJob::StatusCreateInfo &statusCreateInfo);
    void deleteCustomUserStatus(const QByteArray &customUserStatusId);
    void updateCustomUserStatus(const RocketChatRestApi::CustomUserStatusUpdateJob::StatusUpdateInfo &statusUpdateInfo);
    void resetAvatar(const RocketChatRestApi::UserBaseJob::UserInfo &info);
    void statistics(bool refresh);
    void searchRooms(const QString &pattern);
    void getMentionedMessages(const QByteArray &roomId, int offset = 0, int count = 50);
    void hideMentionStatus(const QByteArray &roomId, bool value);
    void historyChannel(const RocketChatRestApi::ChannelHistoryJob::ChannelHistoryInfo &info);

Q_SIGNALS:
    void avatar(const RocketChatRestApi::UserBaseJob::UserInfo &info, const QUrl &url);
    void redownloadAvatar();
    void loginDone(const QString &authToken, const QString &userId);
    void downloadFileDone(const QUrl &url, const QUrl &localFileUrl);
    void serverInfoDone(const QString &version);
    void serverInfoFailed(bool useDeprecatedVersion);
    void getOwnInfoDone(const QJsonObject &data);
    void privateInfoDone(const QJsonObject &data);
    void channelFilesDone(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void channelMembersDone(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void syncThreadMessagesDone(const QJsonObject &obj, const QString &threadMessageId);
    void getRoomsDone(const QJsonObject &obj);
    void userInfoDone(const QJsonObject &obj);
    void channelRolesDone(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void getUsernameSuggestionDone(const QString &username);
    void getPresenceDone(const QString &presence);
    void permissionListAllDone(const QJsonObject &obj);
    void getCommandsDone(const QJsonObject &obj);
    void setJoinCodeDone();
    void channelGetAllUserMentionsDone(const QJsonObject &obj, const QString &roomId);
    void channelKickUserDone(const QJsonObject &obj);
    void groupKickUserDone(const QJsonObject &obj);
    void addModeratorDone();
    void removeModeratorDone();
    void addLeaderDone();
    void removeLeaderDone();
    void addOwnerDone();
    void channelRemoveOwnerDone();
    void groupRemoveOwnerDone();
    void deletechannelDone();
    void deleteGroupsDone();
    void pinMessageDone();
    void ignoreUserDone(const QJsonObject &obj, const QByteArray &roomId);
    void groupRolesDone(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void followMessageDone();
    void unFollowMessageDone();
    void startDiscussionDone();
    void getDiscussionsDone(const QJsonObject &obj, const QByteArray &roomId);
    void getThreadsDone(const QJsonObject &obj, const QString &roomId, bool onlyUnread);
    void getThreadMessagesDone(const QJsonObject &obj, const QByteArray &threadMessageId);
    void getPinnedMessagesDone(const QJsonObject &obj, const QByteArray &roomId);
    void getStarredMessagesDone(const QJsonObject &obj, const QByteArray &roomId);
    void getSnippetedMessagesDone(const QJsonObject &obj, const QByteArray &roomId);
    void getMentionedMessagesDone(const QJsonObject &obj, const QByteArray &roomId);
    void roomsAdminDone(const QJsonObject &obj);
    void translateSavesettingsDone();
    void setStatusDone();
    void usersPresenceDone(const QJsonObject &obj);
    void customUserStatusDone(const QJsonObject &);

    void usersAutocompleteDone(const QJsonObject &obj);

    void failed(const QString &str);
    void networkSessionFailedError();
    void findOrCreateInviteDone(const RocketChatRestApi::FindOrCreateInviteJob::InviteUsersInfo &info);

    void getMessageDone(const QJsonObject &obj, const QByteArray &messageId, const QByteArray &roomId);
    void runCommandDone();
    void sendEmailCodeDone();
    void deleteOwnAccountDone();
    void registerUserDone();
    void enableEmailDone();
    void disableEmailDone();
    void updateOwnBasicInfoDone();
    void cleanHistoryDone();
    void channelGetCountersDone(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void markAsReadDone(const QByteArray &roomId);
    void usersSetPreferencesDone(const QJsonObject &replyObject);
    void createUserStatusDone();
    void userStatusDeletedDone();
    void customUserUpdateDone();
    void postMessageDone(const QJsonObject &replyObject);
    void resetAvatarDone();
    void statisticDone(const QJsonObject &replyObject);
    void updateMessageFailed(const QString &str);
    void forgotPasswordDone();

    void loginStatusChanged();

private:
    Q_DISABLE_COPY(Connection)
    LIBRUQOLACORE_NO_EXPORT void initializeCookies();
    LIBRUQOLACORE_NO_EXPORT void slotResult(QNetworkReply *reply);
    LIBRUQOLACORE_NO_EXPORT void slotSslErrors(QNetworkReply *reply, const QList<QSslError> &error);
    LIBRUQOLACORE_NO_EXPORT void slotLogout();
    LIBRUQOLACORE_NO_EXPORT void slotLogin(const QString &authToken, const QString &userId);
    LIBRUQOLACORE_NO_EXPORT void slotAddJoinCodeToChannel(const QString &channelId, const QString &password);

    QNetworkAccessManager *const mNetworkAccessManager;
    QNetworkCookieJar *const mCookieJar;
    RocketChatRestApi::RestApiMethod *const mRestApiMethod;
    RESTAuthenticationManager *const mRESTAuthenticationManager;
    RocketChatRestApi::AbstractLogger *mRuqolaLogger = nullptr;
    QString mUserId;
    QString mAuthToken;
    QString mUserName;
    QString mPassword;
    bool mSessionFailed = false;
};
