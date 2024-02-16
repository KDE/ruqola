/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "authenticationinfo.h"
#include "bannerinfo/bannerinfos.h"
#include "channelgroupbasejob.h"
#include "commands/runcommandjob.h"
#include "createchannelteaminfo.h"
#include "custom/customuserstatuscreatejob.h"
#include "custom/customuserstatusupdatejob.h"
#include "customusers/customuserstatuses.h"
#include "ddpapi/ddpauthenticationmanager.h"
#include "ddpapi/ddpclient.h"
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
class TypingNotification;
class UsersModel;
class RoomModel;
class MessagesModel;
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
class InputTextManager;
class PluginAuthenticationInterface;
class Room;
class CommonMessagesModel;
class CommonMessageFilterProxyModel;
class ReceiveTypingNotificationManager;
class EmoticonModel;
class DiscussionsFilterProxyModel;
class DiscussionsModel;
class ThreadMessageModel;
class ListMessagesModel;
class ListMessagesFilterProxyModel;
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
class AccountRoomSettings;
class PreviewUrlCacheManager;
class E2eKeyManager;

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

    [[nodiscard]] QList<TextEmoticonsCore::CustomEmoji> customEmojies() const;

    void reconnectToServer();
    [[nodiscard]] Room::TeamRoomInfo roomFromTeamId(const QString &teamId) const;

    [[nodiscard]] QString getUserCurrentMessage(const QString &roomId);
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
    [[nodiscard]] QString avatarUrl(const Utils::AvatarInfo &info);
    [[nodiscard]] QUrl attachmentUrlFromLocalCache(const QString &url);
    void loadHistory(const QString &roomID, bool initial = false, qint64 timeStamp = 0);

    void roomFiles(const QString &roomId, Room::RoomType channelType = Room::RoomType::Unknown);
    void addUserToRoom(const QString &username, const QString &roomId, Room::RoomType channelType);
    void changeDefaultAuthentication(int index);
    void messageSearch(const QString &pattern, const QString &rid, bool userRegularExpression = false);
    [[nodiscard]] InputTextManager *inputTextManager() const;

    [[nodiscard]] InputTextManager *inputThreadMessageTextManager() const;

    [[nodiscard]] QList<Permission> permissions() const;

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

    [[nodiscard]] MessagesModel *messageModelForRoom(const QString &roomID);
    void changeShowOriginalMessage(const QString &roomId, const QString &messageId, bool showOriginal);

    void loadMoreListMessages(const QString &roomId);
    void getListMessages(const QString &roomId, ListMessagesModel::ListMessageType type);

    [[nodiscard]] QUrl urlForLink(const QString &link) const;
    void setUserStatusChanged(const QJsonArray &array);

    void setShowRoomAvatar(bool checked);

    CommonMessageFilterProxyModel *searchMessageFilterProxyModel() const;
    FilesForRoomFilterProxyModel *filesForRoomFilterProxyModel() const;
    ReceiveTypingNotificationManager *receiveTypingNotificationManager() const;
    UserCompleterFilterProxyModel *userCompleterFilterProxyModel() const;

    UsersModel *usersModel() const;
    MessagesModel *threadMessageModel() const;
    EmoticonModel *emoticonModel() const;
    CommandsModel *commandsModel() const;
    AutotranslateLanguagesModel *autoTranslateLanguagesModel() const;
    DiscussionsFilterProxyModel *discussionsFilterProxyModel() const;
    UserCompleterModel *userCompleterModel() const;
    RocketChatAccountSettings *settings() const;

    LocalDatabaseManager *localDatabaseManager() const;

    DDPClient *ddp();
    [[nodiscard]] RoomModel *roomModel() const;
    [[nodiscard]] bool editingMode() const;
    [[nodiscard]] bool sortUnreadOnTop() const;
    [[nodiscard]] OwnUserPreferences::RoomListSortOrder roomListSortOrder() const;

    [[nodiscard]] DDPAuthenticationManager::LoginStatus loginStatus();
    RocketChatRestApi::Connection *restApi();

    [[nodiscard]] Room *room(const QString &roomId);

    // Make it private in future
    void slotInformTypingStatus(const QString &room, bool typing);

    [[nodiscard]] MessageQueue *messageQueue() const;

    [[nodiscard]] RocketChatBackend *rocketChatBackend() const;

    [[nodiscard]] RuqolaLogger *ruqolaLogger() const;

    void parsePublicSettings(const QJsonObject &obj, bool update);

    [[nodiscard]] RuqolaServerConfig *ruqolaServerConfig() const;

    void setUserName(const QString &username);
    [[nodiscard]] QString userName() const;

    void setAccountName(const QString &accountname);
    [[nodiscard]] QString accountName() const;

    void setUserId(const QString &userID);
    [[nodiscard]] QString userId() const;

    void setPassword(const QString &password);
    [[nodiscard]] QString password() const;

    void setTwoFactorAuthenticationCode(const QString &twoFactorAuthenticationCode);
    [[nodiscard]] QString twoFactorAuthenticationCode() const;

    void setAuthToken(const QString &token);
    [[nodiscard]] QString authToken() const;

    void setServerUrl(const QString &serverUrl);

    void sendNotification(const QJsonArray &contents);
    void parseOtr(const QJsonArray &contents);

    void setServerVersion(const QString &version);
    [[nodiscard]] QString serverVersion() const;

    [[nodiscard]] bool needAdaptNewSubscriptionRC60() const;
    [[nodiscard]] EmojiManager *emojiManager() const;
    [[nodiscard]] QString userStatusIconFileName(const QString &id);

    void membersInRoom(const QString &roomId, Room::RoomType channelType);
    void parseUsersForRooms(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);

    void insertCompleterUsers();

    PluginAuthenticationInterface *defaultAuthenticationInterface() const;

    [[nodiscard]] CommonMessagesModel *searchMessageModel() const;

    void updateUser(const QJsonObject &object);

    void removeSettings();

    void rolesChanged(const QJsonArray &contents);

    [[nodiscard]] FilesForRoomModel *filesModelForRoom() const;

    [[nodiscard]] DiscussionsModel *discussionsModel() const;

    void updateThreadMessageList(const Message &m);

    void initializeAccount();
    [[nodiscard]] bool allowEditingMessages() const;
    [[nodiscard]] bool isMessageEditable(const Message &message) const;

    [[nodiscard]] bool otrEnabled() const;

    [[nodiscard]] ListMessagesModel *listMessageModel() const;
    [[nodiscard]] ListMessagesFilterProxyModel *listMessagesFilterProxyModel() const;
    [[nodiscard]] ServerConfigInfo *serverConfigInfo() const;
    [[nodiscard]] QString serverUrl() const;
    [[nodiscard]] StatusModel *statusModel() const;

    void customUsersStatus();

    [[nodiscard]] bool jitsiEnabled() const;
    [[nodiscard]] bool allowMessagePinningEnabled() const;
    [[nodiscard]] bool allowMessageStarringEnabled() const;
    [[nodiscard]] bool allowMessageDeletingEnabled() const;

    [[nodiscard]] bool autoTranslateEnabled() const;
    [[nodiscard]] bool threadsEnabled() const;

    [[nodiscard]] bool discussionEnabled() const;
    [[nodiscard]] QString recordingVideoPath() const;
    [[nodiscard]] QString recordingImagePath() const;

    [[nodiscard]] User fullUserInfo(const QString &userName) const;

    void setAccountEnabled(bool enabled);
    [[nodiscard]] bool accountEnabled() const;

    void insertAvatarUrl(const QString &userId, const QUrl &url);

    [[nodiscard]] AccountRoomSettings *accountRoomSettings() const;

    [[nodiscard]] bool isMessageDeletable(const Message &message) const;
    void joinDiscussion(const QString &roomId, const QString &joinCode);
    void setNameChanged(const QJsonArray &array);
    void setOwnStatus(const User &user);
    [[nodiscard]] User::PresenceStatus presenceStatus() const;

    void getListCommands();
    [[nodiscard]] bool runCommand(const QString &msg, const QString &roomId, const QString &tmid = QString());

    void avatarChanged(const QJsonArray &contents);
    void markMessageAsUnReadFrom(const QString &messageId);
    void markRoomAsUnRead(const QString &roomId);
    void sendUserEmailCode();
    void requestNewPassword(const QString &email);
    [[nodiscard]] bool allowDeleteOwnAccount() const;
    [[nodiscard]] bool registrationFormEnabled() const;
    void registerNewUser(const RocketChatRestApi::RegisterUserJob::RegisterUserInfo &userInfo);
    void deleteOwnAccount(const QString &password);

    [[nodiscard]] bool allowEmailChange() const;

    [[nodiscard]] bool allowPasswordChange() const;

    [[nodiscard]] bool allowUsernameChange() const;

    void updateOwnBasicInfo(const RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo &info);
    [[nodiscard]] RuqolaServerConfig::ServerConfigFeatureTypes serverConfigFeatureTypes() const;
    void parseOwnInfoDone(const QJsonObject &replyObject);
    [[nodiscard]] OwnUser ownUser() const;

    void cleanChannelHistory(const RocketChatRestApi::RoomsCleanHistoryJob::CleanHistoryInfo &info);

    [[nodiscard]] bool broadCastEnabled() const;
    [[nodiscard]] bool encryptionEnabled() const;
    [[nodiscard]] bool uploadFileEnabled() const;
    [[nodiscard]] bool isAdministrator() const;
    [[nodiscard]] bool allowProfileChange() const;
    [[nodiscard]] bool allowAvatarChanged() const;

    [[nodiscard]] bool audioRecorderEnabled() const;
    [[nodiscard]] bool videoRecorderEnabled() const;

    void enable2FaEmailJob(bool enable);
    void logoutFromOtherLocation();
    void createDirectMessages(const QStringList &usernames);
    void setDisplayName(const QString &displayName);
    [[nodiscard]] QString displayName() const;

    void deleteCustomUserStatus(const QJsonArray &replyArray);

    void updateCustomUserStatus(const QJsonArray &replyArray);
    [[nodiscard]] CustomUserStatuses customUserStatuses() const;

    void removeCustomUserStatus(const QString &customUserStatusId);
    void updateCustomUserStatus(const RocketChatRestApi::CustomUserStatusUpdateJob::StatusUpdateInfo &statusUpdateInfo);
    void createCustomUserStatus(const RocketChatRestApi::CustomUserStatusCreateJob::StatusCreateInfo &statusCreateInfo);
    void addMessage(const QJsonObject &replyObject, bool useRestApi = false, bool temporaryMessage = false);
    [[nodiscard]] QStringList highlightWords() const;
    void resetAvatar();
    void setAvatarUrl(const QString &url);
    void exportMessages(const RocketChatRestApi::RoomsExportJob::RoomsExportInfo &info);
    [[nodiscard]] bool hasPermission(const QString &permissionId) const;
    [[nodiscard]] QStringList permissions(const QString &permissionId) const;
    void setUserPreferences(const RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo &info);
    [[nodiscard]] QStringList ownUserPermission() const;
    void downloadAppsLanguages();
    [[nodiscard]] bool hasAutotranslateSupport() const;
    [[nodiscard]] OwnUserPreferences ownUserPreferences() const;
    [[nodiscard]] bool ldapEnabled() const;
    UsersForRoomModel *usersModelForRoom(const QString &roomId) const;

    void deleteUser(const QJsonArray &replyArray);
    MessageCache *messageCache() const;

    [[nodiscard]] bool hideRoles() const;
    [[nodiscard]] bool displayAvatars() const;
    [[nodiscard]] bool teamEnabled() const;
    void openPrivateGroup(const QString &identifier, ChannelTypeInfo typeInfo);

    void deleteEmojiCustom(const QJsonArray &replyArray);
    void addUpdateEmojiCustomList(const QJsonArray &replyArray);
    void setImageUrl(const QUrl &url);
    void updateAvatarCache(const Utils::AvatarInfo &info);
    [[nodiscard]] bool userIsOffline(const QString &name);
    [[nodiscard]] QString userStatusStr(const QString &name);
    [[nodiscard]] OtrManager *otrManager() const;

    // Custom Sound
    void deleteCustomSound(const QJsonArray &replyArray);
    void updateCustomSound(const QJsonArray &replyArray);

    // stdout info
    void addStdoutInfo(const QJsonArray &contents);

    // Roles
    void updateRoles(const QJsonArray &contents);

    // Permissions
    void permissionUpdated(const QJsonArray &replyArray);

    [[nodiscard]] const QList<RoleInfo> &roleInfo() const;

    [[nodiscard]] CustomSoundsManager *customSoundManager() const;

    [[nodiscard]] bool attachmentIsInLocalCache(const QString &url);
    void privateSettingsUpdated(const QJsonArray &replyArray);
    void updateUserInRoom(const QJsonObject &roomData);
    void updateUserData(const QJsonArray &contents);
    [[nodiscard]] bool twoFactorAuthenticationEnabled() const;
    [[nodiscard]] bool twoFactorAuthenticationByEmailEnabled() const;
    [[nodiscard]] bool twoFactorAuthenticationByTOTPEnabled() const;
    void generate2FaTotp(const QJsonObject &obj);
    void totpVerify(const QJsonObject &obj);
    void totpDisabledVerify(const QJsonObject &root);

    [[nodiscard]] const QStringList &searchListCompletion() const;
    void setSearchListCompletion(const QStringList &newSearchListCompletion);

    void setOauthAppAdded(const QJsonObject &obj);

    void setOauthAppUpdated(const QJsonObject &obj);

    [[nodiscard]] SwitchChannelHistoryModel *switchChannelHistoryModel() const;

    [[nodiscard]] bool twoFactorAuthenticationEnforcePasswordFallback() const;
    UploadFileManager *uploadFileManager() const;

    [[nodiscard]] int messageMaximumAllowedSize() const;
    [[nodiscard]] bool messageAllowConvertLongMessagesToAttachment() const;

    [[nodiscard]] bool allowPasswordReset() const;
    [[nodiscard]] const BannerInfos &bannerInfos() const;

    [[nodiscard]] bool useRealName() const;

    [[nodiscard]] bool hasLicense(const QString &name);
    void parseLicenses(const QJsonArray &replyArray);
    // Only for test
    void setOwnUserPreferences(const OwnUserPreferences &ownUserPreferences);

    [[nodiscard]] bool sortFavoriteChannels() const;
    void setShowFavoriteRoom(bool checked);
    void parseVideoConference(const QJsonArray &contents);
    [[nodiscard]] VideoConferenceManager *videoConferenceManager() const;

    [[nodiscard]] VideoConferenceMessageInfoManager *videoConferenceMessageInfoManager() const;

    [[nodiscard]] QUrl faviconLogoUrlFromLocalCache(const QString &url);
    void addMessageToDataBase(const QString &roomName, const Message &message);
    void deleteMessageFromDatabase(const QString &roomName, const QString &messageId);
    void loadAccountSettings();
    void parseCustomSounds(const QJsonArray &obj);

    [[nodiscard]] QList<AuthenticationInfo> authenticationMethodInfos() const;

    [[nodiscard]] QUrl previewUrlFromLocalCache(const QString &url);
    [[nodiscard]] bool previewEmbed() const;
    [[nodiscard]] QUrl avatarUrlFromLocalCache(const QString &url);

    [[nodiscard]] OwnUserPreferences::RoomListDisplay roomListDisplay() const;

    void setRoomListDisplay(OwnUserPreferences::RoomListDisplay roomListDisplay);
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
    void ownUserUiPreferencesChanged();
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
    void wsClosedSocketError();

private:
    Q_DISABLE_COPY(RocketChatAccount)

    LIBRUQOLACORE_NO_EXPORT void slotChannelFilesDone(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    LIBRUQOLACORE_NO_EXPORT void slotChannelGroupRolesDone(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
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
    LIBRUQOLACORE_NO_EXPORT void fillAuthenticationModel();
    LIBRUQOLACORE_NO_EXPORT void initializeAuthenticationPlugins();
    LIBRUQOLACORE_NO_EXPORT void setDefaultAuthentication(AuthenticationManager::AuthMethodType type);
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
    inputAutocomplete(const QString &roomId, const QString &pattern, const QString &exceptions, InputTextManager::CompletionForType type, bool threadDialog);
    LIBRUQOLACORE_NO_EXPORT void slotGetListMessagesDone(const QJsonObject &obj, const QString &roomId, ListMessagesModel::ListMessageType type);
    LIBRUQOLACORE_NO_EXPORT void slotUserAutoCompleterDone(const QJsonObject &obj);
    LIBRUQOLACORE_NO_EXPORT void slotListCommandDone(const QJsonObject &obj);
    LIBRUQOLACORE_NO_EXPORT void runCommand(const RocketChatRestApi::RunCommandJob::RunCommandInfo &runCommandInfo);

    LIBRUQOLACORE_NO_EXPORT void slotLoginStatusChanged();
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
    LIBRUQOLACORE_NO_EXPORT void updateCustomEmojiList(bool fetchListCustom);
    LIBRUQOLACORE_NO_EXPORT void slotLoadRoles();
    LIBRUQOLACORE_NO_EXPORT void slotAwayStatusChanged(bool away);
    LIBRUQOLACORE_NO_EXPORT void slotJobFailed(const QString &str);
    LIBRUQOLACORE_NO_EXPORT void checkLicenses();
    LIBRUQOLACORE_NO_EXPORT void parsePublicSettings();

    AccountRoomSettings *const mAccountRoomSettings;

    PluginAuthenticationInterface *mDefaultAuthenticationInterface = nullptr;

    QHash<AuthenticationManager::AuthMethodType, PluginAuthenticationInterface *> mLstPluginAuthenticationInterface;
    QList<AuthenticationInfo> mAccountAvailableAuthenticationMethodInfos;
    QList<AuthenticationInfo> mAuthenticationMethodInfos;
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
    InputTextManager *const mInputTextManager;

    InputTextManager *const mInputThreadMessageTextManager;

    CommonMessagesModel *mSearchMessageModel = nullptr;
    CommonMessageFilterProxyModel *mSearchMessageFilterProxyModel = nullptr;
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
    ListMessagesFilterProxyModel *mListMessagesFilterProxyModel = nullptr;

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
    PreviewUrlCacheManager *const mPreviewUrlCacheManager;
    E2eKeyManager *const mE2eKeyManager;

    int mDelayReconnect = 100;
    bool mEditingMode = false;
    bool mMarkUnreadThreadsAsReadOnNextReply = false;
};
