/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "accountroomsettings.h"
#include "authenticationinfo.h"
#include "bannerinfo/bannerinfos.h"
#include "channelgroupbasejob.h"
#include "commands/runcommandjob.h"
#include "createchannelteaminfo.h"
#include "custom/customuserstatuscreatejob.h"
#include "custom/customuserstatusupdatejob.h"
#include "customusers/customuserstatuses.h"
#include "ddpapi/ddpauthenticationmanager.h"
#include "inputtextmanager.h"
#include "libruqolacore_export.h"
#include "licenses/licensesmanager.h"
#include "messages/message.h"
#include "model/listmessagesmodel.h"
#include "model/roommodel.h"
#include "notificationinfo.h"
#include "ownuser/ownuser.h"
#include "ownuser/ownuserpreferences.h"
#include "parsemessageurlutils.h"
#include "permissions/permissionmanager.h"
#include "rocketchataccountsettings.h"
#include "roles/rolesmanager.h"
#include "room.h"
#include "rooms/roomscleanhistoryjob.h"
#include "rooms/roomsexportjob.h"
#include "ruqolaserverconfig.h"
#include "serverconfiginfo.h"
#include "users/registeruserjob.h"
#include "users/userssetpreferencesjob.h"
#include "users/usersupdateownbasicinfojob.h"
#include "utils.h"
#include <QObject>
#include <QUrl>
#include <TextEmoticonsCore/CustomEmoji>
#include <ddpapi/ddpclient.h>
class TypingNotification;
class UsersModel;
class RoomModel;
class MessageModel;
class DDPClient;
class MessageQueue;
class RocketChatBackend;
class RuqolaLogger;
class RuqolaServerConfig;
class UserCompleterModel;
class UserCompleterFilterProxyModel;
class StatusModel;
class RocketChatCache;
class EmojiManager;
class OtrManager;
class FilesForRoomFilterProxyModel;
class FilesForRoomModel;
class SearchChannelModel;
class SearchChannelFilterProxyModel;
class LoginMethodModel;
class InputTextManager;
class PluginAuthenticationInterface;
class Room;
class SearchMessageModel;
class SearchMessageFilterProxyModel;
class ReceiveTypingNotificationManager;
class EmoticonModel;
class DiscussionsFilterProxyModel;
class DiscussionsModel;
class ThreadMessageModel;
class ListMessagesModel;
class ListMessagesModelFilterProxyModel;
class AutotranslateLanguagesModel;
class CommandsModel;
class DownloadAppsLanguagesManager;
class UsersForRoomModel;
class MessageCache;
class ManageChannels;
class CustomSoundsManager;
class AwayManager;
class SwitchChannelHistoryModel;
class UploadFileManager;
class VideoConferenceManager;
class VideoConferenceMessageInfoManager;
class LocalDatabaseManager;
class ManageLocalDatabase;

namespace RocketChatRestApi
{
class Connection;
}

class LIBRUQOLACORE_EXPORT RocketChatAccount : public QObject
{
    Q_OBJECT
public:
    explicit RocketChatAccount(const QString &accountName = QString(), QObject *parent = nullptr);
    ~RocketChatAccount() override;

    enum ChannelTypeInfo {
        RoomId,
        RoomName,
    };
    Q_ENUM(ChannelTypeInfo)

    enum RoomInfoType {
        Announcement,
        Description,
        Name,
        Topic,
        ReadOnly,
        Archive,
        RoomType,
        Encrypted,
        Password,
    };
    Q_ENUM(RoomInfoType)

    enum NotificationOptionsType {
        DisableNotifications,
        HideUnreadStatus,
        DesktopNotifications,
        DesktopDurationNotifications,
        DesktopSoundNotifications,
        EmailNotifications,
        MobilePushNotifications,
        UnreadAlert,
        MuteGroupMentions,
        HideMentionStatus,
    };

    Q_ENUM(NotificationOptionsType)

    enum RoleType {
        AddOwner,
        AddLeader,
        AddModerator,
        RemoveOwner,
        RemoveLeader,
        RemoveModerator,
    };
    Q_ENUM(RoleType)

    Q_REQUIRED_RESULT QList<TextEmoticonsCore::CustomEmoji> customEmojies() const;

    void reconnectToServer();
    Q_REQUIRED_RESULT Room::TeamRoomInfo roomFromTeamId(const QString &teamId) const;

    Q_REQUIRED_RESULT QString getUserCurrentMessage(const QString &roomId);
    void setUserCurrentMessage(const QString &message, const QString &roomId);
    void textEditing(const QString &roomId, bool clearNotification);
    void leaveRoom(const QString &identifier, Room::RoomType channelType);
    void hideRoom(const QString &roomId, Room::RoomType channelType);
    void tryLogin();
    void logOut();
    void clearAllUnreadMessages();
    void markRoomAsRead(const QString &roomId);
    void changeFavorite(const QString &roomId, bool checked);
    void sendMessage(const QString &roomID, const QString &message);
    void updateMessage(const QString &roomID, const QString &messageId, const QString &message);
    void replyOnThread(const QString &roomID, const QString &threadMessageId, const QString &message);
    void openChannel(const QString &identifier, RocketChatAccount::ChannelTypeInfo typeInfo);
    void joinJitsiConfCall(const QString &roomId);
    void createNewChannel(const RocketChatRestApi::CreateChannelTeamInfo &info);
    void joinRoom(const QString &roomId, const QString &joinCode = QString());
    void openDirectChannel(const QString &username);
    void setDefaultStatus(User::PresenceStatus status, const QString &messageStatus); // Move to private no ?
    void createJitsiConfCall(const QString &roomId);
    void deleteMessage(const QString &messageId, const QString &roomId);
    void userAutocomplete(const QString &searchText, const QString &exception);
    void eraseRoom(const QString &roomId, Room::RoomType channelType);
    void changeNotificationsSettings(const QString &roomId, RocketChatAccount::NotificationOptionsType notificationsType, const QVariant &newValue);
    void downloadFile(const QString &downloadFileUrl, const QUrl &localFile);
    void starMessage(const QString &messageId, bool starred);
    void pinMessage(const QString &messageId, bool pinned);
    Q_REQUIRED_RESULT QString avatarUrl(const Utils::AvatarInfo &info);
    Q_REQUIRED_RESULT QUrl attachmentUrlFromLocalCache(const QString &url);
    void loadHistory(const QString &roomID, bool initial = false, qint64 timeStamp = 0);
    void channelAndPrivateAutocomplete(const QString &pattern);

    void roomFiles(const QString &roomId, Room::RoomType channelType = Room::RoomType::Unknown);
    void addUserToRoom(const QString &username, const QString &roomId, Room::RoomType channelType);
    void changeDefaultAuthentication(int index);
    void messageSearch(const QString &pattern, const QString &rid, bool userRegularExpression = false);
    Q_REQUIRED_RESULT InputTextManager *inputTextManager() const;

    Q_REQUIRED_RESULT InputTextManager *inputThreadMessageTextManager() const;

    Q_REQUIRED_RESULT QVector<Permission> permissions() const;

    void blockUser(const QString &userId, bool block);
    void deleteFileMessage(const QString &roomId, const QString &fileId, Room::RoomType channelType);
    void openDocumentation();
    void clearSearchModel();
    void reactOnMessage(const QString &messageId, const QString &emoji, bool shouldReact);
    void ignoreUser(const QString &rid, const QString &userId, bool ignore);
    void channelInfo(const QString &roomId);
    void groupInfo(const QString &roomId);
    void switchEditingMode(bool b);
    void setSortUnreadOnTop(bool b);
    void setRoomListSortOrder(OwnUserPreferences::RoomListSortOrder roomListSortOrder);
    void kickUser(const QString &rid, const QString &userId, Room::RoomType channelType);
    void changeRoles(const QString &rid, const QString &userId, Room::RoomType channelType, RocketChatAccount::RoleType roleType);
    void rolesInRoom(const QString &roomId, Room::RoomType channelType);
    void switchingToRoom(const QString &roomID);
    void reportMessage(const QString &messageId, const QString &message);
    void getThreadMessages(const QString &threadMessageId, const Message &message);
    void createDiscussion(const QString &parentRoomName,
                          const QString &discussionName,
                          const QString &replyMessage,
                          const QString &messageId,
                          const QStringList &users = QStringList());
    void threadsInRoom(const QString &roomId, bool onlyUnread);
    void discussionsInRoom(const QString &roomId);
    void followMessage(const QString &messageId, bool follow);
    void loadMoreFileAttachments(const QString &roomId, Room::RoomType channelType);
    void loadMoreDiscussions(const QString &roomId);
    void loadThreadMessagesHistory(const QString &roomId);
    void loadMoreUsersInRoom(const QString &roomId, Room::RoomType channelType);

    void getPinnedMessages(const QString &roomId);
    void getStarredMessages(const QString &roomId);
    void getMentionsMessages(const QString &roomId);

    void autoTranslateSaveLanguageSettings(const QString &roomId, const QString &language);
    void autoTranslateSaveAutoTranslateSettings(const QString &roomId, bool autoTranslate);

    Q_REQUIRED_RESULT MessageModel *messageModelForRoom(const QString &roomID);
    void changeShowOriginalMessage(const QString &roomId, const QString &messageId, bool showOriginal);

    void loadMoreListMessages(const QString &roomId);
    void getListMessages(const QString &roomId, ListMessagesModel::ListMessageType type);

    Q_REQUIRED_RESULT QUrl urlForLink(const QString &link) const;
    void setUserStatusChanged(const QJsonArray &array);

    void setShowRoomAvatar(bool checked);

    SearchMessageFilterProxyModel *searchMessageFilterProxyModel() const;
    FilesForRoomFilterProxyModel *filesForRoomFilterProxyModel() const;
    ReceiveTypingNotificationManager *receiveTypingNotificationManager() const;
    UserCompleterFilterProxyModel *userCompleterFilterModelProxy() const;

    UsersModel *usersModel() const;
    MessageModel *threadMessageModel() const;
    EmoticonModel *emoticonModel() const;
    CommandsModel *commandsModel() const;
    SearchChannelFilterProxyModel *searchChannelFilterProxyModel() const;
    AutotranslateLanguagesModel *autoTranslateLanguagesModel() const;
    DiscussionsFilterProxyModel *discussionsFilterProxyModel() const;
    SearchChannelModel *searchChannelModel() const;
    UserCompleterModel *userCompleterModel() const;
    RocketChatAccountSettings *settings() const;

    LocalDatabaseManager *localDatabaseManager() const;

    DDPClient *ddp();
    Q_REQUIRED_RESULT RoomModel *roomModel() const;
    Q_REQUIRED_RESULT LoginMethodModel *loginMethodModel() const;
    Q_REQUIRED_RESULT bool editingMode() const;
    Q_REQUIRED_RESULT bool sortUnreadOnTop() const;
    Q_REQUIRED_RESULT OwnUserPreferences::RoomListSortOrder roomListSortOrder() const;

    Q_REQUIRED_RESULT DDPAuthenticationManager::LoginStatus loginStatus();
    RocketChatRestApi::Connection *restApi();

    Q_REQUIRED_RESULT Room *room(const QString &roomId);

    // Make it private in future
    void slotInformTypingStatus(const QString &room, bool typing);

    Q_REQUIRED_RESULT MessageQueue *messageQueue() const;

    Q_REQUIRED_RESULT RocketChatBackend *rocketChatBackend() const;

    Q_REQUIRED_RESULT RuqolaLogger *ruqolaLogger() const;

    void parsePublicSettings(const QJsonObject &obj);
    // Method used when we update settings after loading data from Database
    void parseUpdatePublicSettings(const QJsonObject &obj);

    Q_REQUIRED_RESULT RuqolaServerConfig *ruqolaServerConfig() const;

    void setUserName(const QString &username);
    Q_REQUIRED_RESULT QString userName() const;

    void setAccountName(const QString &accountname);
    Q_REQUIRED_RESULT QString accountName() const;

    void setUserId(const QString &userID);
    Q_REQUIRED_RESULT QString userId() const;

    void setPassword(const QString &password);
    Q_REQUIRED_RESULT QString password() const;

    void setTwoFactorAuthenticationCode(const QString &twoFactorAuthenticationCode);
    Q_REQUIRED_RESULT QString twoFactorAuthenticationCode() const;

    void setAuthToken(const QString &token);
    Q_REQUIRED_RESULT QString authToken() const;

    void setServerUrl(const QString &serverUrl);

    void sendNotification(const QJsonArray &contents);
    void parseOtr(const QJsonArray &contents);

    void setServerVersion(const QString &version);
    Q_REQUIRED_RESULT QString serverVersion() const;

    Q_REQUIRED_RESULT bool needAdaptNewSubscriptionRC60() const;
    Q_REQUIRED_RESULT EmojiManager *emojiManager() const;
    Q_REQUIRED_RESULT QString userStatusIconFileName(const QString &id);

    void membersInRoom(const QString &roomId, Room::RoomType channelType);
    void parseUsersForRooms(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);

    void insertCompleterUsers();

    PluginAuthenticationInterface *defaultAuthenticationInterface() const;

    Q_REQUIRED_RESULT SearchMessageModel *searchMessageModel() const;

    void updateUser(const QJsonObject &object);

    void removeSettings();

    void rolesChanged(const QJsonArray &contents);

    Q_REQUIRED_RESULT FilesForRoomModel *filesModelForRoom() const;

    Q_REQUIRED_RESULT DiscussionsModel *discussionsModel() const;

    void updateThreadMessageList(const Message &m);

    void initializeAccount();
    Q_REQUIRED_RESULT bool allowEditingMessages() const;
    Q_REQUIRED_RESULT bool isMessageEditable(const Message &message) const;

    Q_REQUIRED_RESULT bool otrEnabled() const;

    Q_REQUIRED_RESULT ListMessagesModel *listMessageModel() const;
    Q_REQUIRED_RESULT ListMessagesModelFilterProxyModel *listMessagesFilterProxyModel() const;
    Q_REQUIRED_RESULT ServerConfigInfo *serverConfigInfo() const;
    Q_REQUIRED_RESULT QString serverUrl() const;
    Q_REQUIRED_RESULT StatusModel *statusModel() const;

    void customUsersStatus();

    Q_REQUIRED_RESULT bool jitsiEnabled() const;
    Q_REQUIRED_RESULT bool allowMessagePinningEnabled() const;
    Q_REQUIRED_RESULT bool allowMessageStarringEnabled() const;
    Q_REQUIRED_RESULT bool allowMessageDeletingEnabled() const;

    Q_REQUIRED_RESULT bool autoTranslateEnabled() const;
    Q_REQUIRED_RESULT bool threadsEnabled() const;

    Q_REQUIRED_RESULT bool discussionEnabled() const;
    Q_REQUIRED_RESULT QString recordingVideoPath() const;
    Q_REQUIRED_RESULT QString recordingImagePath() const;

    Q_REQUIRED_RESULT User fullUserInfo(const QString &userName) const;

    void setAccountEnabled(bool enabled);
    Q_REQUIRED_RESULT bool accountEnabled() const;

    void insertAvatarUrl(const QString &userId, const QUrl &url);

    Q_REQUIRED_RESULT AccountRoomSettings *accountRoomSettings() const;

    Q_REQUIRED_RESULT bool isMessageDeletable(const Message &message) const;
    void joinDiscussion(const QString &roomId, const QString &joinCode);
    void setNameChanged(const QJsonArray &array);
    void setOwnStatus(const User &user);
    Q_REQUIRED_RESULT User::PresenceStatus presenceStatus() const;

    void getListCommands();
    Q_REQUIRED_RESULT bool runCommand(const QString &msg, const QString &roomId, const QString &tmid = QString());

    void avatarChanged(const QJsonArray &contents);
    void markMessageAsUnReadFrom(const QString &messageId);
    void markRoomAsUnRead(const QString &roomId);
    void sendUserEmailCode();
    void requestNewPassword(const QString &email);
    Q_REQUIRED_RESULT bool allowDeleteOwnAccount() const;
    Q_REQUIRED_RESULT bool registrationFromEnabled() const;
    void registerNewUser(const RocketChatRestApi::RegisterUserJob::RegisterUserInfo &userInfo);
    void deleteOwnAccount(const QString &password);

    Q_REQUIRED_RESULT bool allowEmailChange() const;

    Q_REQUIRED_RESULT bool allowPasswordChange() const;

    Q_REQUIRED_RESULT bool allowUsernameChange() const;

    void updateOwnBasicInfo(const RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo &info);
    Q_REQUIRED_RESULT RuqolaServerConfig::ServerConfigFeatureTypes serverConfigFeatureTypes() const;
    void parseOwnInfoDone(const QJsonObject &replyObject);
    Q_REQUIRED_RESULT OwnUser ownUser() const;

    void cleanChannelHistory(const RocketChatRestApi::RoomsCleanHistoryJob::CleanHistoryInfo &info);

    Q_REQUIRED_RESULT bool broadCastEnabled() const;
    Q_REQUIRED_RESULT bool encryptionEnabled() const;
    Q_REQUIRED_RESULT bool uploadFileEnabled() const;
    Q_REQUIRED_RESULT bool isAdministrator() const;
    Q_REQUIRED_RESULT bool allowProfileChange() const;
    Q_REQUIRED_RESULT bool allowAvatarChanged() const;

    Q_REQUIRED_RESULT bool audioRecorderEnabled() const;
    Q_REQUIRED_RESULT bool videoRecorderEnabled() const;

    void enable2FaEmailJob(bool enable);
    void logoutFromOtherLocation();
    void createDirectMessages(const QStringList &usernames);
    void setDisplayName(const QString &displayName);
    Q_REQUIRED_RESULT QString displayName() const;

    void deleteCustomUserStatus(const QJsonArray &replyArray);

    void updateCustomUserStatus(const QJsonArray &replyArray);
    Q_REQUIRED_RESULT CustomUserStatuses customUserStatuses() const;

    void removeCustomUserStatus(const QString &customUserStatusId);
    void updateCustomUserStatus(const RocketChatRestApi::CustomUserStatusUpdateJob::StatusUpdateInfo &statusUpdateInfo);
    void createCustomUserStatus(const RocketChatRestApi::CustomUserStatusCreateJob::StatusCreateInfo &statusCreateInfo);
    void addMessage(const QJsonObject &replyObject, bool useRestApi = false, bool temporaryMessage = false);
    Q_REQUIRED_RESULT QStringList highlightWords() const;
    void resetAvatar();
    void setAvatarUrl(const QString &url);
    void exportMessages(const RocketChatRestApi::RoomsExportJob::RoomsExportInfo &info);
    Q_REQUIRED_RESULT bool hasPermission(const QString &permissionId) const;
    Q_REQUIRED_RESULT QStringList permissions(const QString &permissionId) const;
    void setUserPreferences(const RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo &info);
    Q_REQUIRED_RESULT QStringList ownUserPermission() const;
    void downloadAppsLanguages();
    Q_REQUIRED_RESULT bool hasAutotranslateSupport() const;
    Q_REQUIRED_RESULT OwnUserPreferences ownUserPreferences() const;
    Q_REQUIRED_RESULT bool ldapEnabled() const;
    UsersForRoomModel *usersModelForRoom(const QString &roomId) const;

    void deleteUser(const QJsonArray &replyArray);
    MessageCache *messageCache() const;

    Q_REQUIRED_RESULT bool hideRoles() const;
    Q_REQUIRED_RESULT bool displayAvatars() const;
    Q_REQUIRED_RESULT bool teamEnabled() const;
    void openPrivateGroup(const QString &identifier, ChannelTypeInfo typeInfo);

    void deleteEmojiCustom(const QJsonArray &replyArray);
    void addUpdateEmojiCustomList(const QJsonArray &replyArray);
    void setImageUrl(const QUrl &url);
    void updateAvatarCache(const Utils::AvatarInfo &info);
    Q_REQUIRED_RESULT bool userIsOffline(const QString &name);
    Q_REQUIRED_RESULT QString userStatusStr(const QString &name);
    Q_REQUIRED_RESULT OtrManager *otrManager() const;

    // Custom Sound
    void deleteCustomSound(const QJsonArray &replyArray);
    void updateCustomSound(const QJsonArray &replyArray);

    // stdout info
    void addStdoutInfo(const QJsonArray &contents);

    // Roles
    void updateRoles(const QJsonArray &contents);

    // Permissions
    void permissionUpdated(const QJsonArray &replyArray);

    Q_REQUIRED_RESULT const QVector<RoleInfo> &roleInfo() const;

    Q_REQUIRED_RESULT CustomSoundsManager *customSoundManager() const;

    Q_REQUIRED_RESULT bool attachmentIsInLocalCache(const QString &url);
    void privateSettingsUpdated(const QJsonArray &replyArray);
    void updateUserInRoom(const QJsonObject &roomData);
    void updateUserData(const QJsonArray &contents);
    Q_REQUIRED_RESULT bool twoFactorAuthenticationEnabled() const;
    Q_REQUIRED_RESULT bool twoFactorAuthenticationByEmailEnabled() const;
    Q_REQUIRED_RESULT bool twoFactorAuthenticationByTOTPEnabled() const;
    void generate2FaTotp(const QJsonObject &obj);
    void totpVerify(const QJsonObject &obj);
    void totpDisabledVerify(const QJsonObject &root);

    Q_REQUIRED_RESULT const QStringList &searchListCompletion() const;
    void setSearchListCompletion(const QStringList &newSearchListCompletion);

    void setOauthAppAdded(const QJsonObject &obj);

    void setOauthAppUpdated(const QJsonObject &obj);

    Q_REQUIRED_RESULT SwitchChannelHistoryModel *switchChannelHistoryModel() const;

    Q_REQUIRED_RESULT bool twoFactorAuthenticationEnforcePasswordFallback() const;
    UploadFileManager *uploadFileManager() const;

    Q_REQUIRED_RESULT int messageMaximumAllowedSize() const;
    Q_REQUIRED_RESULT bool messageAllowConvertLongMessagesToAttachment() const;

    Q_REQUIRED_RESULT bool allowPasswordReset() const;
    Q_REQUIRED_RESULT const BannerInfos &bannerInfos() const;

    Q_REQUIRED_RESULT bool useRealName() const;

    Q_REQUIRED_RESULT int messageViewMode() const;
    Q_REQUIRED_RESULT bool hasLicense(const QString &name);
    void parseLicenses(const QJsonArray &replyArray);
    // Only for test
    void setOwnUserPreferences(const OwnUserPreferences &ownUserPreferences);

    Q_REQUIRED_RESULT bool sortFavoriteChannels() const;
    void setShowFavoriteRoom(bool checked);
    void parseVideoConference(const QJsonArray &contents);
    Q_REQUIRED_RESULT VideoConferenceManager *videoConferenceManager() const;

    Q_REQUIRED_RESULT VideoConferenceMessageInfoManager *videoConferenceMessageInfoManager() const;

    Q_REQUIRED_RESULT QUrl faviconLogoUrlFromLocalCache(const QString &url);
    void addMessageToDataBase(const QString &roomName, const Message &message);
    void deleteMessageFromDatabase(const QString &roomName, const QString &messageId);
    void loadAccountSettings();

Q_SIGNALS:
    void roomRemoved(const QString &roomId);
    void disabledTotpValid(bool checked);
    void totpInvalid();
    void totpValid(const QStringList &code);
    void avatarWasChanged(const Utils::AvatarInfo &info);
    void accountInitialized();
    void connectedChanged();
    void accountNameChanged();
    void userNameChanged();
    void userIdChanged();
    void passwordChanged();
    void twoFactorAuthenticationCodeChanged();
    void serverUrlChanged();
    void loginStatusChanged();
    void socketError(QAbstractSocket::SocketError error, const QString &errorString);
    void logoutDone(const QString &accountname);
    void added(const QJsonObject &item);
    void changed(const QJsonObject &item);
    void removed(const QJsonObject &item);
    void notification(const NotificationInfo &info);
    void fileDownloaded(const QString &filePath, const QUrl &cacheImageUrl);
    void updateNotification(bool hasAlert, int nbUnread, const QString &accountName);
    void missingChannelPassword(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void editingModeChanged();
    void jobFailed(const QString &message, const QString &accountName);
    void switchedRooms();
    void userStatusUpdated(User::PresenceStatus status, const QString &customText, const QString &accountName);
    void publicSettingChanged();
    void serverVersionChanged();
    void openLinkRequested(const QString &link);
    void selectRoomByRoomIdRequested(const QString &identifier, const QString &messageId = QString());
    void selectRoomByRoomNameRequested(const QString &identifier);
    void openTeamNameRequested(const QString &identifier);
    void openThreadRequested(const QString &threadMessageId, const QString &threadMessagePreview, bool threadIsFollowing, const Message &message);
    void raiseWindow();
    void registerUserSuccess();
    void roomNeedAttention();
    void ownInfoChanged();
    void customUserStatusChanged();
    void ownUserPreferencesChanged();
    void customStatusChanged();
    void permissionChanged();
    void displayReconnectWidget(int seconds);

    void customSoundRemoved(const QString &identifier);
    void customSoundAdded(); // TODO add argument ?
    void customSoundUpdated(); // TODO add argument ?

    void insertStdOutInfo(const QString &str);
    void rolesUpdated();
    void totpResult(const QString &secret, const QString &url);

    void selectChannelAndMessage(const QString &messageId,
                                 const QString &roomId,
                                 ParseMessageUrlUtils::RoomIdType roomType,
                                 ParseMessageUrlUtils::ChannelType channelType);

    void oauthAppAdded(const QJsonObject &obj);
    void oauthAppUpdated(const QJsonObject &obj);
    void needUpdateMessageView();
    void publicSettingLoaded(const QJsonObject &obj);
    void bannerInfoChanged();
    void privateSettingsChanged();
    void updateMessageFailed(const QString &str);

private:
    Q_DISABLE_COPY(RocketChatAccount)

    LIBRUQOLACORE_NO_EXPORT void slotChannelFilesDone(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    LIBRUQOLACORE_NO_EXPORT void slotChannelGroupRolesDone(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    LIBRUQOLACORE_NO_EXPORT void slotSplotLightDone(const QJsonObject &obj);
    LIBRUQOLACORE_NO_EXPORT void slotChannelListDone(const QJsonObject &obj);
    LIBRUQOLACORE_NO_EXPORT void slotGetThreadMessagesDone(const QJsonObject &obj, const QString &threadMessageId);
    LIBRUQOLACORE_NO_EXPORT void slotGetDiscussionsListDone(const QJsonObject &obj, const QString &roomId);
    LIBRUQOLACORE_NO_EXPORT void slotGetSupportedLanguagesDone(const QJsonObject &obj);
    LIBRUQOLACORE_NO_EXPORT void slotUsersPresenceDone(const QJsonObject &obj);
    LIBRUQOLACORE_NO_EXPORT void slotReconnectToServer();

    LIBRUQOLACORE_NO_EXPORT void loadEmoji(const QJsonObject &obj);
    LIBRUQOLACORE_NO_EXPORT void slotSearchMessages(const QJsonObject &obj);
    LIBRUQOLACORE_NO_EXPORT void slotNeedToUpdateNotification();
    LIBRUQOLACORE_NO_EXPORT void loadSettings(const QString &accountFileName);
    LIBRUQOLACORE_NO_EXPORT void clearModels();
    LIBRUQOLACORE_NO_EXPORT void fillOauthModel();
    LIBRUQOLACORE_NO_EXPORT void initializeAuthenticationPlugins();
    LIBRUQOLACORE_NO_EXPORT void setDefaultAuthentication(AuthenticationManager::OauthType type);
    LIBRUQOLACORE_NO_EXPORT void userStatusChanged(const User &user);
    LIBRUQOLACORE_NO_EXPORT void openArchivedRoom(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    LIBRUQOLACORE_NO_EXPORT void slotChannelGetCountersDone(const QJsonObject &obj,
                                                            const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);

    LIBRUQOLACORE_NO_EXPORT void getSupportedLanguages();
    LIBRUQOLACORE_NO_EXPORT void usersPresence();
    LIBRUQOLACORE_NO_EXPORT void listEmojiCustom();

    LIBRUQOLACORE_NO_EXPORT void checkInitializedRoom(const QString &roomId);
    LIBRUQOLACORE_NO_EXPORT void clearTypingNotification();
    LIBRUQOLACORE_NO_EXPORT void
    inputAutocomplete(const QString &pattern, const QString &exceptions, InputTextManager::CompletionForType type, bool threadDialog);
    LIBRUQOLACORE_NO_EXPORT void slotGetListMessagesDone(const QJsonObject &obj, const QString &roomId, ListMessagesModel::ListMessageType type);
    LIBRUQOLACORE_NO_EXPORT void slotUserAutoCompleterDone(const QJsonObject &obj);
    LIBRUQOLACORE_NO_EXPORT void slotListCommandDone(const QJsonObject &obj);
    LIBRUQOLACORE_NO_EXPORT void runCommand(const RocketChatRestApi::RunCommandJob::RunCommandInfo &runCommandInfo);

    LIBRUQOLACORE_NO_EXPORT void loginStatusChangedSlot();
    LIBRUQOLACORE_NO_EXPORT void slotFileLanguagedParsed();
    LIBRUQOLACORE_NO_EXPORT void slotRegisterUserDone();
    LIBRUQOLACORE_NO_EXPORT void slotRoomNeedAttention();
    LIBRUQOLACORE_NO_EXPORT void slotMarkAsReadDone(const QString &roomId);
    LIBRUQOLACORE_NO_EXPORT void slotCustomUserStatusDone(const QJsonObject &customList);
    LIBRUQOLACORE_NO_EXPORT void slotPostMessageDone(const QJsonObject &replyObject);
    LIBRUQOLACORE_NO_EXPORT void slotRoomExportDone();
    LIBRUQOLACORE_NO_EXPORT void slotPermissionListAllDone(const QJsonObject &replyObject);
    LIBRUQOLACORE_NO_EXPORT void slotUsersSetPreferencesDone(const QJsonObject &replyObject);
    LIBRUQOLACORE_NO_EXPORT void slotUpdateCustomUserStatus();
    LIBRUQOLACORE_NO_EXPORT void slotDirectoryDone(const QJsonObject &obj);
    LIBRUQOLACORE_NO_EXPORT void updateCustomEmojiList(bool fetchListCustom);
    LIBRUQOLACORE_NO_EXPORT void slotLoadRoles();
    LIBRUQOLACORE_NO_EXPORT void slotAwayStatusChanged(bool away);
    LIBRUQOLACORE_NO_EXPORT void slotJobFailed(const QString &str);
    LIBRUQOLACORE_NO_EXPORT void checkLicenses();
    LIBRUQOLACORE_NO_EXPORT void parsePublicSettings();

    AccountRoomSettings *const mAccountRoomSettings;

    PluginAuthenticationInterface *mDefaultAuthenticationInterface = nullptr;

    QHash<AuthenticationManager::OauthType, PluginAuthenticationInterface *> mLstPluginAuthenticationInterface;
    QVector<AuthenticationInfo> mLstInfos;
    RocketChatAccountSettings *mSettings = nullptr;

    EmojiManager *mEmojiManager = nullptr;
    TypingNotification *mTypingNotification = nullptr;
    UsersModel *const mUserModel;
    RoomModel *const mRoomModel;
    DDPClient *mDdp = nullptr;
    RocketChatRestApi::Connection *mRestApi = nullptr;
    MessageQueue *mMessageQueue = nullptr;
    RocketChatBackend *mRocketChatBackend = nullptr;
    RuqolaLogger *mRuqolaLogger = nullptr;
    RuqolaServerConfig *const mRuqolaServerConfig;
    UserCompleterModel *const mUserCompleterModel;
    UserCompleterFilterProxyModel *mUserCompleterFilterModelProxy = nullptr;
    StatusModel *const mStatusModel;
    RocketChatCache *mCache = nullptr;
    OtrManager *const mOtrManager;
    SearchChannelModel *const mSearchChannelModel;
    SearchChannelFilterProxyModel *mSearchChannelFilterProxyModel = nullptr;
    LoginMethodModel *const mLoginMethodModel;
    InputTextManager *const mInputTextManager;

    InputTextManager *const mInputThreadMessageTextManager;

    SearchMessageModel *mSearchMessageModel = nullptr;
    SearchMessageFilterProxyModel *mSearchMessageFilterProxyModel = nullptr;
    ReceiveTypingNotificationManager *const mReceiveTypingNotificationManager;
    ServerConfigInfo *mServerConfigInfo = nullptr;
    FilesForRoomModel *mFilesModelForRoom = nullptr;
    FilesForRoomFilterProxyModel *mFilesForRoomFilterProxyModel = nullptr;
    DiscussionsFilterProxyModel *mDiscussionsFilterProxyModel = nullptr;
    DiscussionsModel *const mDiscussionsModel;

    EmoticonModel *mEmoticonModel = nullptr;
    CommandsModel *const mCommandsModel;
    ThreadMessageModel *mThreadMessageModel = nullptr;

    ListMessagesModel *mListMessageModel = nullptr;
    ListMessagesModelFilterProxyModel *mListMessagesFilterProxyModel = nullptr;

    AutotranslateLanguagesModel *const mAutoTranslateLanguagesModel;
    User::PresenceStatus mPresenceStatus = User::PresenceStatus::PresenceOnline;
    DownloadAppsLanguagesManager *const mDownloadAppsLanguagesManager;
    MessageCache *const mMessageCache;
    ManageChannels *const mManageChannels;
    CustomSoundsManager *const mCustomSoundManager;
    AwayManager *const mAwayManager;
    SwitchChannelHistoryModel *const mSwitchChannelHistoryModel;
    UploadFileManager *const mUploadFileManager;
    OwnUser mOwnUser;
    CustomUserStatuses mCustomUserStatuses;
    PermissionManager mPermissionManager;
    RolesManager mRolesManager;
    LicensesManager mLicensesManager;
    QStringList mSearchListCompletion;
    BannerInfos mBannerInfos;
    VideoConferenceManager *const mVideoConferenceManager;
    VideoConferenceMessageInfoManager *const mVideoConferenceMessageInfoManager;
    std::unique_ptr<LocalDatabaseManager> mLocalDatabaseManager;
    ManageLocalDatabase *const mManageLoadHistory;

    int mDelayReconnect = 100;
    bool mEditingMode = false;
    bool mMarkUnreadThreadsAsReadOnNextReply = false;
};
