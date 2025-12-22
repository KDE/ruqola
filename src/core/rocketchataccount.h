/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "authenticationinfo.h"
#include "bannerinfo/bannerinfos.h"
#include "channelgroupbasejob.h"

#include "customusers/customuserstatuses.h"

#include "ddpapi/ddpclient.h"
#include "downloadappslanguages/downloadappslanguagesinfo.h"
#include "inputtextmanager.h"
#include "libruqolacore_export.h"
#include "licenses/licensesmanager.h"
#include "model/listmessagesmodel.h"
#include "notificationinfo.h"
#include "ownuser/ownuser.h"
#include "ownuser/ownuserpreferences.h"
#include "parserocketchaturlutils.h"
#include "permissions/permissionsmanager.h"

#include "roles/rolesmanager.h"
#include "room.h"
#include "users/registeruserjob.h"
#include "users/userssetpreferencesjob.h"
#include <QObject>
#include <QUrl>
#include <TextEmoticonsCore/CustomEmoji>
class TypingNotification;
class ActionButtonsManager;
class UsersModel;
class RoomModel;
class MessagesModel;
class DDPClient;
class MessageQueue;
class RocketChatBackend;
class RuqolaLogger;
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
class ReceiveTypingNotificationManager;
class EmoticonModel;
class DiscussionsFilterProxyModel;
class DiscussionsModel;
class ThreadMessageModel;
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
class Connection;
class NotificationPreferences;
class RocketChatAccountSettings;
class SoundManager;
class AppsMarketPlaceModel;
class AppsCategoriesModel;
class MemoryManager;
class ServerConfigInfo;
class RuqolaServerConfig;

namespace RocketChatRestApi
{
struct CreateChannelTeamInfo;
}

class LIBRUQOLACORE_EXPORT RocketChatAccount : public QObject
{
    Q_OBJECT
public:
    explicit RocketChatAccount(const QString &accountName = QString(), QObject *parent = nullptr);
    ~RocketChatAccount() override;

    enum class ChannelTypeInfo : uint8_t {
        RoomId,
        RoomName,
    };
    Q_ENUM(ChannelTypeInfo)

    enum class RoomInfoType : uint8_t {
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

    enum class NotificationOptionsType : uint8_t {
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

    enum class RoleType : uint8_t {
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
    [[nodiscard]] Room::TeamRoomInfo roomFromTeamId(const QByteArray &teamId) const;

    void textEditing(const QByteArray &roomId, bool clearNotification);
    void leaveRoom(const QByteArray &identifier, Room::RoomType channelType);
    void tryLogin();
    void logOut();
    void clearAllUnreadMessages();
    void markRoomAsRead(const QByteArray &roomId);
    void changeFavorite(const QByteArray &roomId, bool checked);
    void sendMessage(const QByteArray &roomID, const QString &message);
    void updateMessage(const QByteArray &roomID, const QByteArray &messageId, const QString &message);
    void replyOnThread(const QByteArray &roomID, const QByteArray &threadMessageId, const QString &message);
    void openChannel(const QString &identifier, RocketChatAccount::ChannelTypeInfo typeInfo);
    void joinJitsiConfCall(const QByteArray &roomId);
    void createNewChannel(const RocketChatRestApi::CreateChannelTeamInfo &info);
    void joinRoom(const QString &roomId, const QString &joinCode = QString());
    void openDirectChannel(const QString &username);
    void setDefaultStatus(User::PresenceStatus status, const QString &messageStatus); // Move to private no ?
    void createJitsiConfCall(const QByteArray &roomId);
    void userAutocomplete(const QString &searchText, const QString &exception);
    void eraseRoom(const QByteArray &roomId, Room::RoomType channelType);
    void changeNotificationsSettings(const QByteArray &QByteArray, RocketChatAccount::NotificationOptionsType notificationsType, const QVariant &newValue);
    void downloadFile(const QString &downloadFileUrl, const QUrl &localFile);
    [[nodiscard]] QString avatarUrl(const Utils::AvatarInfo &info);
    [[nodiscard]] QUrl attachmentUrlFromLocalCache(const QString &url);
    void loadHistory(const QByteArray &roomID, bool initial = false, qint64 timeStamp = 0);

    void roomFiles(const QByteArray &roomId, Room::RoomType channelType = Room::RoomType::Unknown);
    void addUserToRoom(const QByteArray &username, const QByteArray &roomId, Room::RoomType channelType);
    [[nodiscard]] InputTextManager *inputTextManager() const;

    [[nodiscard]] InputTextManager *inputThreadMessageTextManager() const;

    [[nodiscard]] QList<Permission> permissions() const;

    void blockUser(const QString &userId, bool block);
    void reactOnMessage(const QByteArray &messageId, const QString &emoji, bool shouldReact);
    void ignoreUser(const QByteArray &rid, const QByteArray &userId, bool ignore);
    void setSortUnreadOnTop(bool b);
    void setRoomListSortOrder(OwnUserPreferences::RoomListSortOrder roomListSortOrder);
    void kickUser(const QByteArray &rid, const QByteArray &userId, Room::RoomType channelType);
    void changeRoles(const QByteArray &rid, const QString &userId, Room::RoomType channelType, RocketChatAccount::RoleType roleType);
    void switchingToRoom(const QByteArray &roomID);
    void getThreadMessages(const QByteArray &threadMessageId, const Message &message);
    void discussionsInRoom(const QByteArray &roomId);
    void loadMoreFileAttachments(const QByteArray &roomId, Room::RoomType channelType);
    void loadMoreDiscussions(const QByteArray &roomId);
    void loadThreadMessagesHistory(const QByteArray &threadMessageId);
    void loadMoreUsersInRoom(const QByteArray &roomId, Room::RoomType channelType, const QString &filter);
    void loadMoreListMessages(const QByteArray &roomId);

    [[nodiscard]] MessagesModel *messageModelForRoom(const QByteArray &roomID);

    void getListMessages(const QByteArray &roomId, ListMessagesModel::ListMessageType type);

    [[nodiscard]] QUrl urlForLink(const QString &link) const;
    void setUserStatusChanged(const QJsonArray &array);

    void setShowRoomAvatar(bool checked);

    FilesForRoomFilterProxyModel *filesForRoomFilterProxyModel() const;
    ReceiveTypingNotificationManager *receiveTypingNotificationManager() const;
    UserCompleterFilterProxyModel *userCompleterFilterProxyModel() const;

    UsersModel *usersModel() const;
    MessagesModel *threadMessageModel() const;
    AutotranslateLanguagesModel *autoTranslateLanguagesModel() const;
    DiscussionsFilterProxyModel *discussionsFilterProxyModel() const;
    RocketChatAccountSettings *settings() const;

    LocalDatabaseManager *localDatabaseManager() const;

    DDPClient *ddp();
    [[nodiscard]] RoomModel *roomModel() const;
    [[nodiscard]] bool sortUnreadOnTop() const;
    [[nodiscard]] OwnUserPreferences::RoomListSortOrder roomListSortOrder() const;

    [[nodiscard]] AuthenticationManager::LoginStatus loginStatus() const;
    [[nodiscard]] QString loginStatusText() const;
    Connection *restApi();

    [[nodiscard]] Room *room(const QByteArray &roomId) const;

    [[nodiscard]] RocketChatBackend *rocketChatBackend() const;

    [[nodiscard]] RuqolaLogger *ruqolaLogger() const;

    void parsePublicSettings(const QJsonObject &obj, bool update);

    [[nodiscard]] RuqolaServerConfig *ruqolaServerConfig() const;

    [[nodiscard]] QString userName() const;

    void setAccountName(const QString &accountname);
    [[nodiscard]] QString accountName() const;

    [[nodiscard]] QByteArray userId() const;

    void setServerUrl(const QString &serverUrl);

    void sendNotification(const QJsonArray &contents);
    void parseOtr(const QJsonArray &contents);

    void setServerVersion(const QString &version);

    [[nodiscard]] EmojiManager *emojiManager() const;
    [[nodiscard]] QString userStatusIconFileName(const QString &id);

    void removeSettings();

    void rolesChanged(const QJsonArray &contents);

    [[nodiscard]] FilesForRoomModel *filesModelForRoom() const;

    [[nodiscard]] DiscussionsModel *discussionsModel() const;

    void updateThreadMessageList(const Message &m);

    void initializeAccount();
    [[nodiscard]] bool isMessageEditable(const Message &message) const;

    [[nodiscard]] ListMessagesModel *listMessageModel() const;
    [[nodiscard]] ListMessagesFilterProxyModel *listMessagesFilterProxyModel() const;
    [[nodiscard]] ServerConfigInfo *serverConfigInfo() const;
    [[nodiscard]] QString serverUrl() const;
    [[nodiscard]] StatusModel *statusModel() const;

    [[nodiscard]] QString recordingVideoPath() const;
    [[nodiscard]] QString recordingImagePath() const;

    [[nodiscard]] bool accountEnabled() const;

    void insertAvatarUrl(const QString &userId, const QUrl &url);

    [[nodiscard]] AccountRoomSettings *accountRoomSettings() const;

    [[nodiscard]] bool isMessageDeletable(const Message &message) const;
    void setNameChanged(const QJsonArray &array);
    [[nodiscard]] User::PresenceStatus presenceStatus() const;

    void avatarChanged(const QJsonArray &contents);
    void registerNewUser(const RocketChatRestApi::RegisterUserJob::RegisterUserInfo &userInfo);

    void parseOwnInfoDone(const QJsonObject &replyObject);
    [[nodiscard]] OwnUser ownUser() const;

    void deleteCustomUserStatus(const QJsonArray &replyArray);

    void updateCustomUserStatus(const QJsonArray &replyArray);
    [[nodiscard]] CustomUserStatuses customUserStatuses() const;

    void addMessage(const QJsonObject &replyObject, bool useRestApi = false, bool temporaryMessage = false);
    [[nodiscard]] QStringList highlightWords() const;
    void setAvatarUrl(const QString &url);
    [[nodiscard]] bool hasPermission(const QString &permissionId, const QByteArray &roomId = {}) const;
    [[nodiscard]] QStringList permissions(const QString &permissionId) const;
    void setUserPreferences(const RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo &info);
    [[nodiscard]] QStringList ownUserPermission() const;
    [[nodiscard]] bool hasAutotranslateSupport() const;
    [[nodiscard]] OwnUserPreferences ownUserPreferences() const;
    UsersForRoomModel *usersModelForRoom(const QByteArray &roomId) const;

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
    void generate2FaTotp(const QJsonObject &obj);
    void totpVerify(const QJsonObject &obj);
    void totpDisabledVerify(const QJsonObject &root);

    [[nodiscard]] const QStringList &searchListCompletion() const;
    void setSearchListCompletion(const QStringList &newSearchListCompletion);
    [[nodiscard]] SwitchChannelHistoryModel *switchChannelHistoryModel() const;

    UploadFileManager *uploadFileManager() const;

    [[nodiscard]] const BannerInfos &bannerInfos() const;

    [[nodiscard]] bool hasLicense(const QString &name);
    // Only for test
    void setOwnUserPreferences(const OwnUserPreferences &ownUserPreferences);

    [[nodiscard]] bool sortFavoriteChannels() const;
    void setShowFavoriteRoom(bool checked);
    void parseVideoConference(const QJsonArray &contents);
    [[nodiscard]] VideoConferenceMessageInfoManager *videoConferenceMessageInfoManager() const;

    [[nodiscard]] QUrl faviconLogoUrlFromLocalCache(const QString &url);
    void addMessageToDataBase(const QByteArray &roomId, const Message &message);
    void deleteMessageFromDatabase(const QByteArray &roomId, const QByteArray &messageId);
    void loadAccountSettings();

    [[nodiscard]] QList<AuthenticationInfo> authenticationMethodInfos() const;

    [[nodiscard]] QUrl previewUrlFromLocalCache(const QString &url);
    [[nodiscard]] QUrl avatarUrlFromLocalCache(const QString &url);
    [[nodiscard]] QUrl soundUrlFromLocalCache(const QString &url);

    [[nodiscard]] OwnUserPreferences::RoomListDisplay roomListDisplay() const;

    void setRoomListDisplay(OwnUserPreferences::RoomListDisplay roomListDisplay);

    [[nodiscard]] NotificationPreferences *notificationPreferences() const;

    void playSound(const QByteArray &soundIdentifier);
    void playNewRoomNotification();

    [[nodiscard]] AppsMarketPlaceModel *appsMarketPlaceModel() const;
    [[nodiscard]] AppsCategoriesModel *appsCategoriesModel() const;

    void loadAppMarketPlace();
    void loadAppCategories();
    void loadInstalledApps();

    [[nodiscard]] bool appMarketPlaceLoaded() const;
    [[nodiscard]] E2eKeyManager *e2eKeyManager() const;

    [[nodiscard]] MemoryManager *memoryManager() const;

    [[nodiscard]] bool e2EPasswordMustBeSave() const;
    void setE2EPasswordMustBeSave(bool newE2EPasswordMustBeSave);

    [[nodiscard]] bool e2EPasswordMustBeDecrypt() const;

    void setLastSelectedRoom(const QByteArray &roomId);

    void streamNotifyUserOtrEnd(const QByteArray &roomId, const QByteArray &userId);

    void muteUser(const QByteArray &rid, const QString &userId, bool mute);
    void initializeDirectChannel(const QByteArray &rid);
    void delaySelectChannelRequested(const QByteArray &rid);
    [[nodiscard]] bool hasAtLeastVersion(int major, int minor, int patch) const;
    void extractIdentifier(const QJsonObject &replyObject, const QString &identifier, const QString &subIdentifier);
    void loadAppCount();

    void getRoomByTypeAndName(const QByteArray &rid, const QString &roomType);

    [[nodiscard]] QString getTranslatedIdentifier(const QString &lang, const QString &identifier) const;

    [[nodiscard]] bool isAdministrator() const;
    void updateInstalledApps();
    void updateApps(const QJsonArray &contents);
    void updateCountApplications();

    [[nodiscard]] bool commandHasPreview(const QString &commandName) const;
    [[nodiscard]] ActionButtonsManager *actionButtonsManager() const;

    [[nodiscard]] QMap<QString, DownloadAppsLanguagesInfo> languagesAppsMap() const;

    [[nodiscard]] bool isFileDeletable(const QByteArray &roomId, const QByteArray &fileUserId, qint64 uploadedAt) const;

    void deleteRoomFromDatabase(const QByteArray &roomId);
    void addRoomToDataBase(Room *r);
    void updateRoomInDatabase(const QByteArray &roomId);
    void addMessagesToDataBase(const QByteArray &roomId, const QList<Message> &messages);

    void updateTextToSpeech(const QByteArray &roomId, const QByteArray &messageId, bool inProgress);

    [[nodiscard]] bool offlineMode() const;

    [[nodiscard]] qint64 globalRoomsTimeStamp() const;
Q_SIGNALS:
    void showUiInteraction(const QJsonArray &uiInteraction);
    void roomRemoved(const QByteArray &roomId);
    void disabledTotpValid(bool checked);
    void totpInvalid();
    void totpValid(const QStringList &code);
    void avatarWasChanged(const Utils::AvatarInfo &info);
    void accountInitialized();
    void ddpConnectedChanged(bool connected);
    void accountNameChanged();
    void userIdChanged();
    void passwordAvailable();
    void twoFactorAuthenticationCodeChanged();
    void loginStatusChanged();
    void ddpLoginStatusChanged();
    void socketError(QAbstractSocket::SocketError error, const QString &errorString);
    void logoutDone(const QString &accountname);
    void notification(const NotificationInfo &info);
    void fileDownloaded(const QString &filePath, const QUrl &cacheImageUrl);
    void updateNotification(bool hasAlert, int nbUnread, const QString &accountName);
    void missingChannelPassword(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void jobFailed(const QString &message, const QString &accountName);
    void userStatusUpdated(User::PresenceStatus status, const QString &customText, const QString &accountName);
    void publicSettingChanged();
    void serverVersionChanged();
    void openLinkRequested(const QString &link);
    void selectRoomByRoomIdRequested(const QByteArray &identifier, const QByteArray &messageId = QByteArray());
    void selectRoomByRoomNameRequested(const QString &identifier);
    void openTeamNameRequested(const QByteArray &identifier);
    void openThreadRequested(const QByteArray &threadMessageId, const QString &threadMessagePreview, bool threadIsFollowing, const Message &message);
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

    void customSoundRemoved(const QByteArray &identifier);
    void customSoundAdded(const QByteArray &identifier);
    void customSoundUpdated(const QByteArray &identifier);

    void insertStdOutInfo(const QString &str);
    void rolesUpdated();
    void totpResult(const QString &secret, const QString &url);

    void selectChannelAndMessage(const QByteArray &messageId,
                                 const QString &roomId,
                                 ParseRocketChatUrlUtils::RoomIdType roomType,
                                 ParseRocketChatUrlUtils::ChannelType channelType);

    void needUpdateMessageView();
    void publicSettingLoaded(const QJsonObject &obj);
    void bannerInfoChanged();
    void privateSettingsChanged();
    void updateMessageFailed(const QString &str);
    void activitiesChanged();
    void appsMarkPlaceLoadDone();
    void needToSaveE2EPassword();
    void needToDecryptE2EPassword();
    void permissionSettingLoaded(const QJsonObject &obj);
    void privateSettingLoaded(const QJsonObject &obj);
    void appsCountLoadDone();
    void offlineModeChanged();

private:
    LIBRUQOLACORE_NO_EXPORT void setE2EPasswordMustBeDecrypt(bool newE2EPasswordMustBeDecrypt);

    LIBRUQOLACORE_NO_EXPORT void rolesInRoom(const QByteArray &roomId, Room::RoomType channelType);

    LIBRUQOLACORE_NO_EXPORT void getPinnedMessages(const QByteArray &roomId);
    LIBRUQOLACORE_NO_EXPORT void getStarredMessages(const QByteArray &roomId);
    LIBRUQOLACORE_NO_EXPORT void getMentionsMessages(const QByteArray &roomId);
    LIBRUQOLACORE_NO_EXPORT void parseUsersForRooms(const QJsonObject &obj, const QByteArray &channelInfoIdentifier, const QString &filter);

    LIBRUQOLACORE_NO_EXPORT void threadsInRoom(const QByteArray &roomId, bool onlyUnread);

    LIBRUQOLACORE_NO_EXPORT void membersInRoom(const QByteArray &roomId, Room::RoomType channelType, const QString &filter);

    LIBRUQOLACORE_NO_EXPORT void slotInformTypingStatus(const QByteArray &room, bool typing);

    LIBRUQOLACORE_NO_EXPORT void setOwnStatus(const User &user);

    LIBRUQOLACORE_NO_EXPORT void startConnecting();
    LIBRUQOLACORE_NO_EXPORT void slotChannelFilesDone(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    LIBRUQOLACORE_NO_EXPORT void slotChannelGroupRolesDone(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    LIBRUQOLACORE_NO_EXPORT void slotGetThreadMessagesDone(const QJsonObject &obj, const QByteArray &threadMessageId);
    LIBRUQOLACORE_NO_EXPORT void slotGetDiscussionsListDone(const QJsonObject &obj, const QByteArray &roomId);
    LIBRUQOLACORE_NO_EXPORT void slotGetSupportedLanguagesDone(const QJsonObject &obj);
    LIBRUQOLACORE_NO_EXPORT void slotUsersPresenceDone(const QJsonObject &obj);
    LIBRUQOLACORE_NO_EXPORT void autoReconnectDelayed();

    LIBRUQOLACORE_NO_EXPORT void loadEmoji(const QJsonObject &obj);
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

    LIBRUQOLACORE_NO_EXPORT void forceDisconnect();
    LIBRUQOLACORE_NO_EXPORT void logoutCompleted();
    LIBRUQOLACORE_NO_EXPORT void getSupportedLanguages();
    LIBRUQOLACORE_NO_EXPORT void listEmojiCustom();

    LIBRUQOLACORE_NO_EXPORT void checkInitializedRoom(const QByteArray &roomId);
    LIBRUQOLACORE_NO_EXPORT void clearTypingNotification();
    LIBRUQOLACORE_NO_EXPORT void
    inputAutocomplete(const QByteArray &roomId, const QString &pattern, const QString &exceptions, InputTextManager::CompletionForType type, bool threadDialog);
    LIBRUQOLACORE_NO_EXPORT void slotGetListMessagesDone(const QJsonObject &obj, const QByteArray &roomId, ListMessagesModel::ListMessageType type);
    LIBRUQOLACORE_NO_EXPORT void slotUserAutoCompleterDone(const QJsonObject &obj);
    LIBRUQOLACORE_NO_EXPORT void slotListCommandDone(const QJsonObject &obj);

    LIBRUQOLACORE_NO_EXPORT void slotLoginStatusChanged();
    LIBRUQOLACORE_NO_EXPORT void slotUpdateCommands();
    LIBRUQOLACORE_NO_EXPORT void slotRegisterUserDone();
    LIBRUQOLACORE_NO_EXPORT void slotRoomNeedAttention();
    LIBRUQOLACORE_NO_EXPORT void slotMarkAsReadDone(const QByteArray &roomId);
    LIBRUQOLACORE_NO_EXPORT void slotCustomUserStatusDone(const QJsonObject &customList);
    LIBRUQOLACORE_NO_EXPORT void slotPostMessageDone(const QJsonObject &replyObject);
    LIBRUQOLACORE_NO_EXPORT void slotPermissionListAllDone(const QJsonObject &replyObject);
    LIBRUQOLACORE_NO_EXPORT void slotUsersSetPreferencesDone(const QJsonObject &replyObject);
    LIBRUQOLACORE_NO_EXPORT void slotUpdateCustomUserStatus();
    LIBRUQOLACORE_NO_EXPORT void updateCustomEmojiList(bool fetchListCustom);
    LIBRUQOLACORE_NO_EXPORT void loadRoles();
    LIBRUQOLACORE_NO_EXPORT void slotAwayStatusChanged(bool away);
    LIBRUQOLACORE_NO_EXPORT void slotJobFailed(const QString &str);
    LIBRUQOLACORE_NO_EXPORT void checkLicenses();
    LIBRUQOLACORE_NO_EXPORT void parsePublicSettings();
    LIBRUQOLACORE_NO_EXPORT void licenseGetModules();
    LIBRUQOLACORE_NO_EXPORT void loadSoundFiles();
    LIBRUQOLACORE_NO_EXPORT void slotReconnectToDdpServer();
    LIBRUQOLACORE_NO_EXPORT void slotVerifyKeysDone();
    LIBRUQOLACORE_NO_EXPORT void slotDDpLoginStatusChanged();
    LIBRUQOLACORE_NO_EXPORT void slotRESTLoginStatusChanged();
    LIBRUQOLACORE_NO_EXPORT void slotRoomOpenChanged(const QByteArray &rid);
    LIBRUQOLACORE_NO_EXPORT void slotCreateChannelDone(const QJsonObject &replyObject);
    LIBRUQOLACORE_NO_EXPORT void slotCreateGroupDone(const QJsonObject &replyObject);
    LIBRUQOLACORE_NO_EXPORT void parseMethodRequested(const QJsonObject &obj, DDPClient::MethodRequestedType type);
    LIBRUQOLACORE_NO_EXPORT void processPublicsettings(const QJsonObject &obj);
    LIBRUQOLACORE_NO_EXPORT void processUpdatePublicsettings(const QJsonObject &obj);
    LIBRUQOLACORE_NO_EXPORT void openRoom(const QJsonObject &obj);
    LIBRUQOLACORE_NO_EXPORT void openDirectChannel(const QJsonObject &root);
    LIBRUQOLACORE_NO_EXPORT void createJitsiConfCall(const QJsonObject &root);
    LIBRUQOLACORE_NO_EXPORT void inputUserChannelAutocompleteThread(const QJsonObject &root);
    LIBRUQOLACORE_NO_EXPORT void inputUserChannelAutocomplete(const QJsonObject &root);
    LIBRUQOLACORE_NO_EXPORT void otrEnd(const QJsonObject &root);
    LIBRUQOLACORE_NO_EXPORT void enable2fa(const QJsonObject &root);
    LIBRUQOLACORE_NO_EXPORT void regenerateCodes2fa(const QJsonObject &root);
    LIBRUQOLACORE_NO_EXPORT void disable2fa(const QJsonObject &root);
    LIBRUQOLACORE_NO_EXPORT void validateTempToken2fa(const QJsonObject &root);
    LIBRUQOLACORE_NO_EXPORT void listCustomSounds(const QJsonObject &root);

    LIBRUQOLACORE_NO_EXPORT void processPublicsettingsAdministrator(const QJsonObject &obj);
    LIBRUQOLACORE_NO_EXPORT void processPrivatesettingsAdministrator(const QJsonObject &obj);
    LIBRUQOLACORE_NO_EXPORT void processPermissionsAdministrator(const QJsonObject &obj);
    LIBRUQOLACORE_NO_EXPORT void roomsParsing(const QJsonObject &root);
    LIBRUQOLACORE_NO_EXPORT void getsubscriptionParsing(const QJsonObject &root);
    LIBRUQOLACORE_NO_EXPORT void displayLogInfo(const QByteArray &ba, const QJsonObject &obj);
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString restLoginStatusText() const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString ddpLoginStatusText() const;
    LIBRUQOLACORE_NO_EXPORT void slotFileLanguagesDownloaded();
    LIBRUQOLACORE_NO_EXPORT void customUserStatus();
    LIBRUQOLACORE_NO_EXPORT void connectOfflineMode();
    LIBRUQOLACORE_NO_EXPORT void loadRoomsFromDatabase();

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
    std::unique_ptr<DDPClient> mDdp;
    std::unique_ptr<Connection> mRestApi;
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
    User::PresenceStatus mPresenceStatus = User::PresenceStatus::Unknown;
    DownloadAppsLanguagesManager *const mDownloadAppsLanguagesManager;
    MessageCache *const mMessageCache;
    ManageChannels *const mManageChannels;
    CustomSoundsManager *const mCustomSoundManager;
    AwayManager *const mAwayManager;
    SwitchChannelHistoryModel *const mSwitchChannelHistoryModel;
    UploadFileManager *const mUploadFileManager;
    OwnUser mOwnUser;
    CustomUserStatuses mCustomUserStatuses;
    PermissionsManager mPermissionManager;
    RolesManager mRolesManager;
    LicensesManager mLicensesManager;
    QStringList mSearchListCompletion;
    BannerInfos mBannerInfos;
    VideoConferenceManager *const mVideoConferenceManager;
    VideoConferenceMessageInfoManager *const mVideoConferenceMessageInfoManager;
    std::unique_ptr<LocalDatabaseManager> mLocalDatabaseManager;
    ManageLocalDatabase *const mManageLocalDatabase;
    PreviewUrlCacheManager *const mPreviewUrlCacheManager;
    NotificationPreferences *const mNotificationPreferences;
    E2eKeyManager *const mE2eKeyManager;
    SoundManager *const mSoundManager;
    AppsMarketPlaceModel *const mAppsMarketPlaceModel;
    AppsCategoriesModel *const mAppsCategoriesModel;
    MemoryManager *const mMemoryManager;
    ActionButtonsManager *const mActionButtonsManager;
    int mDelayReconnect = 100;
    qint64 mAccountTimeStamp = -1;
    bool mMarkUnreadThreadsAsReadOnNextReply = false;
    bool mE2EPasswordMustBeSave = false;
    bool mE2EPasswordMustBeDecrypt = false;
};
