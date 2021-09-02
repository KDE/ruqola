/*
   Copyright (c) 2017-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#pragma once

#include "accountroomsettings.h"
#include "authenticationinfo.h"
#include "channelgroupbasejob.h"
#include "commands/runcommandjob.h"
#include "createroominfo.h"
#include "custom/customuserstatuscreatejob.h"
#include "custom/customuserstatusupdatejob.h"
#include "customuserstatuses.h"
#include "ddpapi/ddpauthenticationmanager.h"
#include "file.h"
#include "inputtextmanager.h"
#include "libruqolacore_export.h"
#include "messages/message.h"
#include "model/listmessagesmodel.h"
#include "model/roommodel.h"
#include "ownuser.h"
#include "permissionmanager.h"
#include "rocketchataccountsettings.h"
#include "room.h"
#include "rooms/roomscleanhistoryjob.h"
#include "rooms/roomsexportjob.h"
#include "ruqolaserverconfig.h"
#include "serverconfiginfo.h"
#include "uploadfilejob.h"
#include "users/registeruserjob.h"
#include "users/userssetpreferencesjob.h"
#include "users/usersupdateownbasicinfojob.h"
#include "utils.h"
#include <QObject>
#include <QUrl>
#include <ddpapi/ddpclient.h>
class TypingNotification;
class UsersModel;
class RoomModel;
class MessageModel;
class DDPClient;
class MessageQueue;
class RocketChatBackend;
class RoomFilterProxyModel;
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
class EmoticonFilterModel;
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
class EmoticonCustomModel;

namespace RocketChatRestApi
{
class RestApiRequest;
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
        AudioNotifications,
        DesktopNotifications,
        DesktopDurationNotifications,
        DesktopSoundNotifications,
        EmailNotifications,
        MobilePushNotifications,
        UnreadAlert,
        MuteGroupMentions
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

    Q_REQUIRED_RESULT Room::TeamRoomInfo roomFromTeamId(const QString &teamId) const;

    Q_REQUIRED_RESULT QString getUserCurrentMessage(const QString &roomId);
    void setUserCurrentMessage(const QString &message, const QString &roomId);
    void textEditing(const QString &roomId, bool clearNotification);
    void leaveRoom(const QString &roomId, Room::RoomType channelType);
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
    void createNewChannel(const RocketChatRestApi::CreateRoomInfo &info, bool privateRoom);
    void joinRoom(const QString &roomId, const QString &joinCode = QString());
    void openDirectChannel(const QString &username);
    void setDefaultStatus(User::PresenceStatus status, const QString &messageStatus); // Move to private no ?
    void changeDefaultStatus(int index, const QString &messageStatus);
    void createJitsiConfCall(const QString &roomId);
    void deleteMessage(const QString &messageId, const QString &roomId);
    void userAutocomplete(const QString &searchText, const QString &exception);
    void eraseRoom(const QString &roomId, Room::RoomType channelType);
    void changeNotificationsSettings(const QString &roomId, RocketChatAccount::NotificationOptionsType notificationsType, const QVariant &newValue);
    void downloadFile(const QString &downloadFileUrl, const QUrl &localFile);
    void starMessage(const QString &messageId, bool starred);
    void pinMessage(const QString &messageId, bool pinned);
    void
    uploadFile(const QString &roomId, const QString &description, const QString &messageText, const QUrl &fileUrl, const QString &threadMessageId = QString());
    Q_REQUIRED_RESULT QString avatarUrl(const Utils::AvatarInfo &info);
    Q_REQUIRED_RESULT QUrl attachmentUrl(const QString &url);
    void loadHistory(const QString &roomID, bool initial = false, qint64 timeStep = 0);
    void channelAndPrivateAutocomplete(const QString &pattern);

    void roomFiles(const QString &roomId, Room::RoomType channelType = Room::RoomType::Unknown);
    void addUserToRoom(const QString &username, const QString &roomId, Room::RoomType channelType);
    void changeDefaultAuthentication(int index);
    void messageSearch(const QString &pattern, const QString &rid, bool userRegularExpression = false);
    Q_REQUIRED_RESULT InputTextManager *inputTextManager() const;

    Q_REQUIRED_RESULT InputTextManager *inputThreadMessageTextManager() const;

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
    void kickUser(const QString &rid, const QString &userId, Room::RoomType channelType);
    void changeRoles(const QString &rid, const QString &userId, Room::RoomType channelType, RocketChatAccount::RoleType roleType);
    void rolesInRoom(const QString &roomId, Room::RoomType channelType);
    void switchingToRoom(const QString &roomID);
    void reportMessage(const QString &messageId, const QString &message);
    void getThreadMessages(const QString &threadMessageId);
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
    void getSnippetedMessages(const QString &roomId);
    void getMentionsMessages(const QString &roomId);

    void autoTranslateSaveLanguageSettings(const QString &roomId, const QString &language);
    void autoTranslateSaveAutoTranslateSettings(const QString &roomId, bool autoTranslate);

    Q_REQUIRED_RESULT MessageModel *messageModelForRoom(const QString &roomID);
    void changeShowOriginalMessage(const QString &roomId, const QString &messageId, bool showOriginal);

    void loadMoreListMessages(const QString &roomId);
    void getListMessages(const QString &roomId, ListMessagesModel::ListMessageType type);

    Q_REQUIRED_RESULT QUrl urlForLink(const QString &link) const;
    void setUserStatusChanged(const QJsonArray &array);

    SearchMessageFilterProxyModel *searchMessageFilterProxyModel() const;
    FilesForRoomFilterProxyModel *filesForRoomFilterProxyModel() const;
    ReceiveTypingNotificationManager *receiveTypingNotificationManager() const;
    UserCompleterFilterProxyModel *userCompleterFilterModelProxy() const;

    UsersModel *usersModel() const;
    RoomFilterProxyModel *roomFilterProxyModel() const;
    MessageModel *threadMessageModel() const;
    EmoticonFilterModel *emoticonFilterModel() const;
    EmoticonModel *emoticonModel() const;
    CommandsModel *commandsModel() const;
    SearchChannelFilterProxyModel *searchChannelFilterProxyModel() const;
    AutotranslateLanguagesModel *autoTranslateLanguagesModel() const;
    DiscussionsFilterProxyModel *discussionsFilterProxyModel() const;
    SearchChannelModel *searchChannelModel() const;
    UserCompleterModel *userCompleterModel() const;
    RocketChatAccountSettings *settings() const;

    DDPClient *ddp();
    Q_REQUIRED_RESULT RoomModel *roomModel() const;
    Q_REQUIRED_RESULT LoginMethodModel *loginMethodModel() const;
    Q_REQUIRED_RESULT bool editingMode() const;
    Q_REQUIRED_RESULT bool sortUnreadOnTop() const;

    Q_REQUIRED_RESULT DDPAuthenticationManager::LoginStatus loginStatus();
    RocketChatRestApi::RestApiRequest *restApi();

    Q_REQUIRED_RESULT Room *room(const QString &roomId);

    // Make it private in future
    void slotInformTypingStatus(const QString &room, bool typing);

    Q_REQUIRED_RESULT MessageQueue *messageQueue() const;

    Q_REQUIRED_RESULT RocketChatBackend *rocketChatBackend() const;

    Q_REQUIRED_RESULT RuqolaLogger *ruqolaLogger() const;

    void parsePublicSettings(const QJsonObject &obj);

    Q_REQUIRED_RESULT RuqolaServerConfig *ruqolaServerConfig() const;

    void setUserName(const QString &username);
    Q_REQUIRED_RESULT QString userName() const;

    void setAccountName(const QString &servername);
    Q_REQUIRED_RESULT QString accountName() const;

    void setUserId(const QString &userID);
    Q_REQUIRED_RESULT QString userId() const;

    void setPassword(const QString &password);
    Q_REQUIRED_RESULT QString password() const;

    void setUseLdap(bool ldap);
    Q_REQUIRED_RESULT bool useLdap() const;

    void setTwoFactorAuthenticationCode(const QString &twoFactorAuthenticationCode);
    Q_REQUIRED_RESULT QString twoFactorAuthenticationCode() const;

    void setAuthToken(const QString &token);
    Q_REQUIRED_RESULT QString authToken() const;

    void setServerUrl(const QString &serverUrl);

    void sendNotification(const QJsonArray &contents);
    void parseOtr(const QJsonArray &contents);
    void setServerVersion(const QString &version);

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
    Q_REQUIRED_RESULT bool allowMessageSnippetingEnabled() const;
    Q_REQUIRED_RESULT bool allowMessageStarringEnabled() const;
    Q_REQUIRED_RESULT bool allowMessageDeletingEnabled() const;
    Q_REQUIRED_RESULT bool hasStarredMessagesSupport() const;
    Q_REQUIRED_RESULT bool hasSnippetedMessagesSupport() const;

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

    Q_REQUIRED_RESULT bool hasInviteUserSupport() const;
    Q_REQUIRED_RESULT bool isMessageDeletable(const Message &message) const;
    void joinDiscussion(const QString &roomId, const QString &joinCode);
    void setNameChanged(const QJsonArray &array);
    void setOwnStatus(const User &user);
    Q_REQUIRED_RESULT User::PresenceStatus presenceStatus() const;

    void getListCommands();
    Q_REQUIRED_RESULT bool runCommand(const QString &msg, const QString &roomId, const QString &tmid = QString());
    Q_REQUIRED_RESULT bool hasOldSubscriptionSupport() const;

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
    Q_REQUIRED_RESULT bool hideAvatars() const;
    void roomsAutocomplete(const QString &searchText, const QString &exception);
    Q_REQUIRED_RESULT QString loadMessagesHistory(const QString &roomID, qint64 numberOfMessages);
    Q_REQUIRED_RESULT bool teamEnabled() const;
    void openPrivateGroup(const QString &identifier, ChannelTypeInfo typeInfo);
    Q_REQUIRED_RESULT EmoticonCustomModel *emoticonCustomModel() const;

Q_SIGNALS:
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
    void notification(const Utils::NotificationInfo &info);
    void fileDownloaded(const QString &filePath, const QUrl &cacheImageUrl);
    void updateNotification(bool hasAlert, int nbUnread, const QString &accountName);
    void missingChannelPassword(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void editingModeChanged();
    void sortUnreadOnTopChanged();
    void jobFailed(const QString &message);
    void switchedRooms();
    void userStatusUpdated(User::PresenceStatus status, const QString &customText, const QString &accountName);
    void publicSettingChanged();
    void serverVersionChanged();
    void openLinkRequested(const QString &link);
    void selectRoomByRoomIdRequested(const QString &identifier);
    void selectRoomByRoomNameRequested(const QString &identifier);
    void openTeamNameRequested(const QString &identifier);
    void openThreadRequested(const QString &threadMessageId, const QString &threadMessagePreview);
    void raiseWindow();
    void registerUserSuccess();
    void roomNeedAttention();
    void ownInfoChanged();
    void customUserStatusChanged();
    void uploadProgress(const RocketChatRestApi::UploadFileJob::UploadStatusInfo &info);
    void ownUserPreferencesChanged();
    void updateStatusComboBox();
    void permissionChanged();

private:
    Q_DISABLE_COPY(RocketChatAccount)

    void slotChannelFilesDone(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void slotChannelRolesDone(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void slotSplotLightDone(const QJsonObject &obj);
    void slotChannelListDone(const QJsonObject &obj);
    void slotGetThreadMessagesDone(const QJsonObject &obj, const QString &threadMessageId);
    void slotGetDiscussionsListDone(const QJsonObject &obj, const QString &roomId);
    void slotGetSupportedLanguagesDone(const QJsonObject &obj);
    void slotUsersPresenceDone(const QJsonObject &obj);
    void slotSocketError(QAbstractSocket::SocketError error, const QString &errorString);
    void slotReconnectToServer();

    void loadEmoji(const QJsonObject &obj);
    void slotSearchMessages(const QJsonObject &obj);
    void slotNeedToUpdateNotification();
    void loadSettings(const QString &accountFileName);
    void clearModels();
    void fillOauthModel();
    void initializeAuthenticationPlugins();
    void setDefaultAuthentication(AuthenticationManager::OauthType type);
    void userStatusChanged(const User &user);
    void openArchivedRoom(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void slotChannelGetCountersDone(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);

    void getSupportedLanguages();
    void usersPresence();
    void listEmojiCustom();

    void checkInitializedRoom(const QString &roomId);
    void clearTypingNotification();
    void inputAutocomplete(const QString &pattern, const QString &exceptions, InputTextManager::CompletionForType type, bool threadDialog);
    void slotGetListMessagesDone(const QJsonObject &obj, const QString &roomId, ListMessagesModel::ListMessageType type);
    void slotUserAutoCompleterDone(const QJsonObject &obj);
    void slotRoomsAutoCompleteChannelAndPrivateDone(const QJsonObject &obj);
    void slotListCommandDone(const QJsonObject &obj);
    void runCommand(const RocketChatRestApi::RunCommandJob::RunCommandInfo &runCommandInfo);
    void updateUserModel(const User &user);

    void loginStatusChangedSlot();
    void slotFileLanguagedParsed();
    void slotRegisterUserDone();
    void slotRoomNeedAttention();
    void slotMarkAsReadDone(const QString &roomId);
    void slotCustomUserStatusDone(const QJsonObject &customList);
    void slotPostMessageDone(const QJsonObject &replyObject);
    void slotRoomExportDone();
    void slotPermissionListAllDone(const QJsonObject &replyObject);
    void slotUsersSetPreferencesDone(const QJsonObject &replyObject);
    void slotUploadProgress(const RocketChatRestApi::UploadFileJob::UploadStatusInfo &info);
    void slotUpdateCustomUserStatus();
    void slotDirectoryDone(const QJsonObject &obj);

    AccountRoomSettings *const mAccountRoomSettings;

    PluginAuthenticationInterface *mDefaultAuthenticationInterface = nullptr;

    QHash<AuthenticationManager::OauthType, PluginAuthenticationInterface *> mLstPluginAuthenticationInterface;
    QVector<AuthenticationInfo> mLstInfos;
    RocketChatAccountSettings *mSettings = nullptr;

    EmojiManager *mEmojiManager = nullptr;
    TypingNotification *mTypingNotification = nullptr;
    UsersModel *mUserModel = nullptr;
    RoomModel *mRoomModel = nullptr;
    RoomFilterProxyModel *mRoomFilterProxyModel = nullptr;
    DDPClient *mDdp = nullptr;
    RocketChatRestApi::RestApiRequest *mRestApi = nullptr;
    MessageQueue *mMessageQueue = nullptr;
    RocketChatBackend *mRocketChatBackend = nullptr;
    RuqolaLogger *mRuqolaLogger = nullptr;
    RuqolaServerConfig *mRuqolaServerConfig = nullptr;
    UserCompleterModel *mUserCompleterModel = nullptr;
    UserCompleterFilterProxyModel *mUserCompleterFilterModelProxy = nullptr;
    StatusModel *mStatusModel = nullptr;
    RocketChatCache *mCache = nullptr;
    OtrManager *mOtrManager = nullptr;
    SearchChannelModel *mSearchChannelModel = nullptr;
    SearchChannelFilterProxyModel *mSearchChannelFilterProxyModel = nullptr;
    LoginMethodModel *mLoginMethodModel = nullptr;
    InputTextManager *mInputTextManager = nullptr;

    InputTextManager *mInputThreadMessageTextManager = nullptr;

    SearchMessageModel *mSearchMessageModel = nullptr;
    SearchMessageFilterProxyModel *mSearchMessageFilterProxyModel = nullptr;
    ReceiveTypingNotificationManager *mReceiveTypingNotificationManager = nullptr;
    ServerConfigInfo *mServerConfigInfo = nullptr;
    FilesForRoomModel *mFilesModelForRoom = nullptr;
    FilesForRoomFilterProxyModel *mFilesForRoomFilterProxyModel = nullptr;
    DiscussionsFilterProxyModel *mDiscussionsFilterProxyModel = nullptr;
    DiscussionsModel *mDiscussionsModel = nullptr;

    EmoticonFilterModel *mEmoticonFilterModel = nullptr;
    EmoticonModel *mEmoticonModel = nullptr;
    EmoticonCustomModel *mEmoticonCustomModel = nullptr;
    CommandsModel *mCommandsModel = nullptr;
    ThreadMessageModel *mThreadMessageModel = nullptr;

    ListMessagesModel *mListMessageModel = nullptr;
    ListMessagesModelFilterProxyModel *mListMessagesFilterProxyModel = nullptr;

    AutotranslateLanguagesModel *mAutoTranslateLanguagesModel = nullptr;
    User::PresenceStatus mPresenceStatus = User::PresenceStatus::PresenceOnline;
    DownloadAppsLanguagesManager *mDownloadAppsLanguagesManager = nullptr;
    MessageCache *const mMessageCache;
    ManageChannels *const mManageChannels;
    OwnUser mOwnUser;
    CustomUserStatuses mCustomUserStatuses;
    PermissionManager mPermissionManager;

    int mDelayReconnect = 100;
    bool mEditingMode = false;
    bool mMarkUnreadThreadsAsReadOnNextReply = false;
};

