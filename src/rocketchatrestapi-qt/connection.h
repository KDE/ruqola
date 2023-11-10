/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "channels/channelhistoryjob.h"
#include "commands/runcommandjob.h"
#include "createchannelteaminfo.h"
#include "custom/customuserstatuscreatejob.h"
#include "custom/customuserstatusupdatejob.h"
#include "invite/findorcreateinvitejob.h"
#include "librocketchatrestapi-qt_export.h"
#include "rooms/adminroomsjob.h"
#include "rooms/roomsautocompletechannelandprivatejob.h"
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
namespace RocketChatRestApi
{
class RestApiAbstractJob;
class DownloadFileJob;
class AbstractLogger;
class LIBROCKETCHATRESTAPI_QT_EXPORT Connection : public QObject
{
    Q_OBJECT
public:
    explicit Connection(QObject *parent = nullptr);
    ~Connection() override;

    void setRestApiLogger(RocketChatRestApi::AbstractLogger *logger);

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
    void channelList();
    void getAvatar(const RocketChatRestApi::UserBaseJob::UserInfo &info);

    void serverInfo(bool useDeprecatedVersion);
    void getPrivateSettings();
    void getOwnInfo();
    void starMessage(const QString &messageId, bool starred);
    RocketChatRestApi::DownloadFileJob *
    downloadFile(const QUrl &url, const QUrl &localFileUrl, const QByteArray &mimeType = "text/plain", bool requiredAuthentication = true);
    void changeChannelTopic(const QString &roomId, const QString &topic);
    void changeGroupsTopic(const QString &roomId, const QString &topic);
    void changeChannelAnnouncement(const QString &roomId, const QString &announcement);
    void changeGroupsAnnouncement(const QString &roomId, const QString &announcement);
    void postMessage(const QString &roomId, const QString &text);
    void deleteMessage(const QString &roomId, const QString &messageId);
    void createChannels(const RocketChatRestApi::CreateChannelTeamInfo &info);
    void createGroups(const RocketChatRestApi::CreateChannelTeamInfo &info);
    void leaveChannel(const QString &roomId);
    void leaveGroups(const QString &roomId);
    void updateMessage(const QString &roomId, const QString &messageId, const QString &text);
    void reactOnMessage(const QString &messageId, const QString &emoji, bool shouldReact);
    void closeChannel(const QString &roomId, const QString &type);
    void createDirectMessage(const QStringList &userName);
    void historyChannel(const QString &roomId, const QString &type);
    void changeChannelDescription(const QString &roomId, const QString &description);
    void changeGroupsDescription(const QString &roomId, const QString &description);
    void archiveChannel(const QString &roomId, bool archive);
    void archiveGroups(const QString &roomId, bool archive);
    void filesInRoom(const QString &roomId, const QString &type, int offset = 0, int count = 50);
    void addUserInChannel(const QString &roomId, const QString &userId);

    void searchRoomUser(const QString &pattern);
    void searchMessages(const QString &roomId, const QString &pattern, bool useRegularExpression = false);
    void markRoomAsRead(const QString &roomId);
    void getRooms();
    void markAsFavorite(const QString &roomId, bool favorite);
    void addUserInGroup(const QString &roomId, const QString &userId);
    void disableNotifications(const QString &roomId, bool value);
    void hideUnreadStatus(const QString &roomId, bool value);
    void desktopNotifications(const QString &roomId, const QString &value);
    void emailNotifications(const QString &roomId, const QString &value);
    void mobilePushNotifications(const QString &roomId, const QString &value);
    void unreadAlert(const QString &roomId, const QString &value);
    void setAvatar(const UserBaseJob::UserInfo &info, const RocketChatRestApi::SetAvatarJob::SetAvatarInfo &avatarInfo);
    void markRoomAsUnRead(const QString &roomId);
    void markMessageAsUnReadFrom(const QString &messageId);
    void forgotPassword(const QString &email);
    void userInfo(const QString &identifier, bool userName = false);
    void ignoreUser(const QString &roomId, const QString &userId, bool ignore);
    void userPresence(const QString &userId);
    void reportMessage(const QString &messageId, const QString &message);
    void setGroupType(const QString &roomId, bool isPrivate);
    void setChannelType(const QString &roomId, bool isPrivate);
    void getGroupRoles(const QString &roomId);
    void getChannelRoles(const QString &roomId);
    void getUsernameSuggestion();
    void listAllPermissions();
    void fetchMyKeys();
    void setJoinCodeChannel(const QString &roomId, const QString &joinCode);
    void muteGroupMentions(const QString &roomId, bool value);

    void channelInfo(const QString &roomId);

    void syncThreadMessages(const QString &threadMessageId, const QString &timestamp);
    void changeChannelName(const QString &roomId, const QString &newName);
    void changeGroupName(const QString &roomId, const QString &newName);
    void groupInfo(const QString &roomId);
    void channelGetAllUserMentions(const QString &roomId, int offset = 0, int count = 50);
    void openDirectMessage(const QString &userId);
    void channelKick(const QString &roomId, const QString &userId);
    void groupKick(const QString &roomId, const QString &userId);
    void groupAddModerator(const QString &roomId, const QString &userId);
    void groupRemoveModerator(const QString &roomId, const QString &userId);
    void groupAddLeader(const QString &roomId, const QString &userId);
    void groupRemoveLeader(const QString &roomId, const QString &userId);
    void groupAddOwner(const QString &roomId, const QString &userId);
    void groupRemoveOwner(const QString &roomId, const QString &userId);
    void channelAddModerator(const QString &roomId, const QString &userId);
    void channelRemoveModerator(const QString &roomId, const QString &userId);
    void channelAddOwner(const QString &roomId, const QString &userId);
    void channelRemoveOwner(const QString &roomId, const QString &userId);
    void channelDelete(const QString &roomId);
    void groupDelete(const QString &roomId);
    void membersInRoom(const QString &roomId, const QString &type, int offset = 0, int count = 50);
    void changeChannelReadOnly(const QString &roomId, bool b);
    void changeGroupsReadOnly(const QString &roomId, bool b);
    void channelAddLeader(const QString &roomId, const QString &userId);
    void channelRemoveLeader(const QString &roomId, const QString &userId);
    void changeGroupsEncrypted(const QString &roomId, bool b);
    void pinMessage(const QString &messageId, bool pinned);
    void desktopDurationNotifications(const QString &roomId, int value);
    void desktopSoundNotifications(const QString &roomId, const QString &value);
    void followMessage(const QString &messageId);
    void unFollowMessage(const QString &messageId);
    void createDiscussion(const QString &parentRoomId,
                          const QString &discussionName,
                          const QString &replyMessage,
                          const QString &parentMessageId,
                          const QStringList &users);
    void getDiscussions(const QString &roomId, int offset = 0, int count = 50);
    void getThreadsList(const QString &roomId, bool onlyUnread, int offset = 0, int count = 50);
    void getThreadMessages(const QString &threadMessageId);
    void sendMessage(const QString &roomId, const QString &text, const QString &messageId = QString(), const QString &threadMessageId = QString());
    void getPinnedMessages(const QString &roomId, int offset = 0, int count = 50);
    void getSupportedLanguagesMessages(bool needTagetLanguage);
    void autoTranslateSaveLanguageSettings(const QString &roomId, const QString &language);
    void autoTranslateSaveAutoTranslateSettings(const QString &roomId, bool autoTranslate);
    void setUserStatus(const QString &userId, SetStatusJob::StatusType status, const QString &message = QString());
    void usersPresence();
    void getStarredMessages(const QString &roomId, int offset = 0, int count = 50);
    void getSnippetedMessages(const QString &roomId, int offset = 0, int count = 50);
    void customUserStatus();
    void usersAutocomplete(const UsersAutocompleteJob::UsersAutocompleterInfo &info);
    void findOrCreateInvite(const QString &roomId, int maxUses, int numberOfDays);
    void getMessage(const QString &messageId, const QString &roomId);
    void getCommand(const QString &commandName);
    void runCommand(const RunCommandJob::RunCommandInfo &runCommandInfo);
    void sendUserEmailCode(const QString &identifier);
    void deleteOwnAccount(const QString &password);
    void registerNewUser(const RocketChatRestApi::RegisterUserJob::RegisterUserInfo &userInfo);
    void enable2FaEmailJob(bool enable);
    void updateOwnBasicInfo(const RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo &info);
    void cleanChannelHistory(const RocketChatRestApi::RoomsCleanHistoryJob::CleanHistoryInfo &info);
    void getRoomsAdmin(const RocketChatRestApi::AdminRoomsJob::AdminRoomsJobInfo &info);
    void getChannelsCounter(const QString &roomId);
    void setUserPreferences(const RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo &info);
    void removeOtherTokens();
    void createCustomUserStatus(const CustomUserStatusCreateJob::StatusCreateInfo &statusCreateInfo);
    void deleteCustomUserStatus(const QString &customUserStatusId);
    void updateCustomUserStatus(const CustomUserStatusUpdateJob::StatusUpdateInfo &statusUpdateInfo);
    void resetAvatar(const UserBaseJob::UserInfo &info);
    void statistics(bool refresh);
    void searchRooms(const QString &pattern);
    void getMentionedMessages(const QString &roomId, int offset = 0, int count = 50);
    void hideMentionStatus(const QString &roomId, bool value);
    void historyChannel(const RocketChatRestApi::ChannelHistoryJob::ChannelHistoryInfo &info);

Q_SIGNALS:
    void avatar(const UserBaseJob::UserInfo &info, const QUrl &url);
    void redownloadAvatar();
    void loginDone(const QString &authToken, const QString &userId);
    void downloadFileDone(const QUrl &url, const QUrl &localFileUrl);
    void serverInfoDone(const QString &version);
    void serverInfoFailed(bool useDeprecatedVersion);
    void getOwnInfoDone(const QJsonObject &data);
    void privateInfoDone(const QJsonObject &data);
    void channelFilesDone(const QJsonObject &obj, const ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void channelMembersDone(const QJsonObject &obj, const ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void syncThreadMessagesDone(const QJsonObject &obj, const QString &threadMessageId);
    void spotlightDone(const QJsonObject &obj);
    void channelListDone(const QJsonObject &obj);
    void searchMessageDone(const QJsonObject &obj);
    void getRoomsDone(const QJsonObject &obj);
    void userInfoDone(const QJsonObject &obj);
    void channelRolesDone(const QJsonObject &obj, const ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void getUsernameSuggestionDone(const QString &username);
    void getPresenceDone(const QString &presence);
    void permissionListAllDone(const QJsonObject &obj);
    void getCommandsDone(const QJsonObject &obj);
    void fetchMyKeysDone();
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
    void ignoreUserDone(const QJsonObject &obj, const QString &roomId);
    void groupRolesDone(const QJsonObject &obj, const ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void followMessageDone();
    void unFollowMessageDone();
    void startDiscussionDone();
    void getDiscussionsDone(const QJsonObject &obj, const QString &roomId);
    void getThreadsDone(const QJsonObject &obj, const QString &roomId, bool onlyUnread);
    void getThreadMessagesDone(const QJsonObject &obj, const QString &threadMessageId);
    void getPinnedMessagesDone(const QJsonObject &obj, const QString &roomId);
    void getStarredMessagesDone(const QJsonObject &obj, const QString &roomId);
    void getSnippetedMessagesDone(const QJsonObject &obj, const QString &roomId);
    void getMentionedMessagesDone(const QJsonObject &obj, const QString &roomId);
    void getSupportedLanguagesDone(const QJsonObject &obj);
    void roomsAdminDone(const QJsonObject &obj);
    void translateSavesettingsDone();
    void setStatusDone();
    void usersPresenceDone(const QJsonObject &obj);
    void customUserStatusDone(const QJsonObject &);

    void usersAutocompleteDone(const QJsonObject &obj);

    void failed(const QString &str);
    void networkSessionFailedError();
    void findOrCreateInviteDone(const RocketChatRestApi::FindOrCreateInviteJob::InviteUsersInfo &info);

    void getMessageDone(const QJsonObject &obj, const QString &messageId, const QString &roomId);
    void runCommandDone();
    void sendEmailCodeDone();
    void deleteOwnAccountDone();
    void registerUserDone();
    void enableEmailDone();
    void disableEmailDone();
    void updateOwnBasicInfoDone();
    void cleanHistoryDone();
    void channelGetCountersDone(const QJsonObject &obj, const ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void markAsReadDone(const QString &roomId);
    void usersSetPreferencesDone(const QJsonObject &replyObject);
    void removeOtherTokensDone();
    void createUserStatusDone();
    void userStatusDeletedDone();
    void customUserUpdateDone();
    void postMessageDone(const QJsonObject &replyObject);
    void resetAvatarDone();
    void statisticDone(const QJsonObject &replyObject);
    void directoryDone(const QJsonObject &replyObject);
    void updateMessageFailed(const QString &str);

private:
    Q_DISABLE_COPY(Connection)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void initializeCookies();
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void slotResult(QNetworkReply *reply);
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void slotSslErrors(QNetworkReply *reply, const QList<QSslError> &error);
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void slotLogout();
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void slotLogin(const QString &authToken, const QString &userId);
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void slotAddJoinCodeToChannel(const QString &channelId, const QString &password);

    QNetworkAccessManager *mNetworkAccessManager = nullptr;
    QNetworkCookieJar *mCookieJar = nullptr;
    RestApiMethod *mRestApiMethod = nullptr;
    RocketChatRestApi::AbstractLogger *mRuqolaLogger = nullptr;
    QString mUserId;
    QString mAuthToken;
    QString mUserName;
    QString mPassword;
    bool mSessionFailed = false;
};
}
