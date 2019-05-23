/*
   Copyright (c) 2017-2019 Montel Laurent <montel@kde.org>

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

#ifndef ROCKETCHATACCOUNT_H
#define ROCKETCHATACCOUNT_H

#include <QObject>
#include <QUrl>
#include <ddpapi/ddpclient.h>
#include "rocketchataccountsettings.h"
#include "libruqola_private_export.h"
#include "authenticationinfo.h"
#include "file.h"
class TypingNotification;
class UsersModel;
class RoomModel;
class RoomWrapper;
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
class UsersForRoomFilterProxyModel;
class UsersForRoomModel;
class FilesForRoomFilterProxyModel;
class SearchChannelModel;
class SearchChannelFilterProxyModel;
class LoginMethodModel;
class InputCompleterModel;
class InputTextManager;
class PluginAuthenticationInterface;
class Room;
class SearchMessageModel;
class SearchMessageFilterProxyModel;
class DiscussionsFilterProxyModel;
class ThreadsFilterProxyModel;
class ServerConfigInfo;
class ReceiveTypingNotificationManager;
class EmoticonModel;
class FilesForRoomModel;
namespace RocketChatRestApi {
class RestApiRequest;
}

class LIBRUQOLACORE_EXPORT RocketChatAccount : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString userID READ userID WRITE setUserID NOTIFY userIDChanged)
    Q_PROPERTY(QString serverUrl READ serverUrl WRITE setServerUrl NOTIFY serverUrlChanged)
    Q_PROPERTY(QString accountName READ accountName WRITE setAccountName NOTIFY accountNameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(DDPClient::LoginStatus loginStatus READ loginStatus NOTIFY loginStatusChanged)
    Q_PROPERTY(bool editingMode READ editingMode NOTIFY editingModeChanged)
public:
    explicit RocketChatAccount(const QString &accountName = QString(), QObject *parent = nullptr);
    ~RocketChatAccount();

    enum RoomInfoType {
        Announcement,
        Description,
        Name,
        Topic,
        ReadOnly,
        Archive,
        RoomType,
        Encrypted,
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
        RemoveModerator
    };
    Q_ENUM(RoleType)

    Q_INVOKABLE UsersModel *usersModel() const;

    Q_INVOKABLE RoomModel *roomModel() const;

    Q_INVOKABLE RoomFilterProxyModel *roomFilterProxyModel() const;
    Q_INVOKABLE UsersForRoomFilterProxyModel *usersForRoomFilterProxyModel(const QString &roomId) const;
    Q_INVOKABLE RoomWrapper *getRoomWrapper(const QString &roomId);
    Q_INVOKABLE MessageModel *messageModelForRoom(const QString &roomID);
    Q_INVOKABLE QString getUserCurrentMessage(const QString &roomId);
    Q_INVOKABLE void setUserCurrentMessage(const QString &message, const QString &roomId);

    Q_INVOKABLE void textEditing(const QString &roomId, const QString &str);

    Q_INVOKABLE void leaveRoom(const QString &roomId, const QString &channelType);
    Q_INVOKABLE void hideRoom(const QString &roomId, const QString &channelType = QString());
    Q_INVOKABLE void tryLogin();
    Q_INVOKABLE void logOut();
    Q_INVOKABLE void clearUnreadMessages(const QString &roomId);

    Q_INVOKABLE void changeFavorite(const QString &roomId, bool checked);
    Q_INVOKABLE void sendMessage(const QString &roomID, const QString &message);
    Q_INVOKABLE void updateMessage(const QString &roomID, const QString &messageId, const QString &message);
    Q_INVOKABLE void openChannel(const QString &url);
    Q_INVOKABLE void joinJitsiConfCall(const QString &roomId);
    Q_INVOKABLE void createNewChannel(const QString &name, bool readOnly, bool privateRoom, const QString &userNames, bool encryptedRoom, const QString &password, bool broadcast);
    Q_INVOKABLE void joinRoom(const QString &roomId, const QString &joinCode = QString());
    Q_INVOKABLE void openDirectChannel(const QString &username);
    Q_INVOKABLE void listEmojiCustom();
    Q_INVOKABLE void setDefaultStatus(User::PresenceStatus status);
    Q_INVOKABLE void changeDefaultStatus(int index);
    Q_INVOKABLE void createJitsiConfCall(const QString &roomId);
    Q_INVOKABLE void deleteMessage(const QString &messageId, const QString &roomId);
    Q_INVOKABLE void userAutocomplete(const QString &searchText, const QString &exception);
    Q_INVOKABLE void eraseRoom(const QString &roomId, const QString &channelType);
    Q_INVOKABLE void changeChannelSettings(const QString &roomId, RocketChatAccount::RoomInfoType infoType, const QVariant &newValue, const QString &channelType = QString());
    Q_INVOKABLE void changeNotificationsSettings(const QString &roomId, RocketChatAccount::NotificationOptionsType notificationsType, const QVariant &newValue);
    Q_INVOKABLE QString recordingVideoPath() const;
    Q_INVOKABLE QString recordingImagePath() const;
    Q_INVOKABLE void downloadFile(const QString &downloadFileUrl, const QUrl &localFile);
    Q_INVOKABLE void starMessage(const QString &messageId, bool starred);
    Q_INVOKABLE void pinMessage(const QString &messageId, bool pinned);

    Q_INVOKABLE void uploadFile(const QString &roomId, const QString &description, const QString &messageText, const QUrl &fileUrl);
    Q_INVOKABLE QString serverUrl() const;

    Q_INVOKABLE QString avatarUrl(const QString &userId);

    Q_INVOKABLE StatusModel *statusModel() const;

    Q_INVOKABLE Q_REQUIRED_RESULT QUrl attachmentUrl(const QString &url);
    Q_INVOKABLE void loadHistory(const QString &roomID, const QString &channelType = QString(), bool initial = false);
    Q_INVOKABLE Q_REQUIRED_RESULT bool allowEditingMessages() const;
    Q_INVOKABLE Q_REQUIRED_RESULT bool otrEnabled() const;
    Q_INVOKABLE void channelAndPrivateAutocomplete(const QString &pattern);
    Q_INVOKABLE UserCompleterFilterProxyModel *userCompleterFilterModelProxy() const;
    Q_INVOKABLE void roomFiles(const QString &roomId, const QString &channelType = QString());
    Q_INVOKABLE FilesForRoomFilterProxyModel *filesForRoomFilterProxyModel() const;
    Q_INVOKABLE void addUserToRoom(const QString &username, const QString &roomId, const QString &channelType);
    Q_INVOKABLE SearchChannelFilterProxyModel *searchChannelFilterProxyModel() const;
    Q_INVOKABLE InputCompleterModel *inputCompleterModel() const;
    Q_INVOKABLE LoginMethodModel *loginMethodModel() const;
    Q_INVOKABLE Room *getRoom(const QString &roomId);

    Q_INVOKABLE DiscussionsFilterProxyModel *discussionsFilterProxyModel(const QString &roomId) const;

    Q_INVOKABLE ThreadsFilterProxyModel *threadsFilterProxyModel(const QString &roomId) const;

    Q_INVOKABLE void changeDefaultAuthentication(int index);
    Q_INVOKABLE void messageSearch(const QString &pattern, const QString &rid);

    Q_INVOKABLE SearchMessageFilterProxyModel *searchMessageFilterProxyModel() const;
    Q_INVOKABLE void setInputTextChanged(const QString &str, int position);
    Q_INVOKABLE QString replaceWord(const QString &newWord, const QString &str, int position);
    Q_INVOKABLE void blockUser(const QString &userId, bool block);
    Q_INVOKABLE QString avatarUrlFromDirectChannel(const QString &rid);
    Q_INVOKABLE void deleteFileMessage(const QString &roomId, const QString &fileId, const QString &channelType);
    Q_INVOKABLE void openDocumentation();
    Q_INVOKABLE void clearSearchModel();

    Q_INVOKABLE void reactOnMessage(const QString &messageId, const QString &emoji, bool shouldReact);
    Q_INVOKABLE void ignoreUser(const QString &rid, const QString &userId, bool ignore);
    Q_INVOKABLE ReceiveTypingNotificationManager *receiveTypingNotificationManager() const;
    Q_INVOKABLE Q_REQUIRED_RESULT bool encryptedEnabled() const;
    Q_INVOKABLE Q_REQUIRED_RESULT QString serverVersionStr() const;
    Q_INVOKABLE Q_REQUIRED_RESULT ServerConfigInfo *serverConfigInfo() const;
    Q_INVOKABLE Q_REQUIRED_RESULT bool jitsiEnabled() const;

    Q_INVOKABLE Q_REQUIRED_RESULT bool allowMessagePinningEnabled() const;
    Q_INVOKABLE Q_REQUIRED_RESULT bool allowMessageSnippetingEnabled() const;
    Q_INVOKABLE Q_REQUIRED_RESULT bool allowMessageStarringEnabled() const;
    Q_INVOKABLE Q_REQUIRED_RESULT bool allowMessageDeletingEnabled() const;

    Q_INVOKABLE void channelInfo(const QString &roomId);
    Q_INVOKABLE void groupInfo(const QString &roomId);
    Q_INVOKABLE void channelGetAllUserMentions(const QString &roomId);

    Q_INVOKABLE void switchEditingMode(bool b);
    Q_INVOKABLE void kickUser(const QString &rid, const QString &userId, const QString &channelType);
    Q_INVOKABLE void changeRoles(const QString &rid, const QString &userId, const QString &channelType, RocketChatAccount::RoleType roleType);
    Q_INVOKABLE void rolesInRoom(const QString &roomId, const QString &channelType);

    Q_INVOKABLE void switchingToRoom(const QString &roomID);
    Q_INVOKABLE void reportMessage(const QString &messageId, const QString &message);
    Q_INVOKABLE void getThreadMessages(const QString &threadMessageId);

    Q_INVOKABLE void createDiscussion(const QString &parentRoomName, const QString &discussionName, const QString &replyMessage, const QString &messageId);

    Q_INVOKABLE Q_REQUIRED_RESULT bool threadsEnabled() const;

    Q_INVOKABLE Q_REQUIRED_RESULT bool discussionEnabled() const;

    Q_INVOKABLE void threadsInRoom(const QString &roomId);
    Q_INVOKABLE void discussionsInRoom(const QString &roomId);

    SearchChannelModel *searchChannelModel() const;
    UserCompleterModel *userCompleterModel() const;
    RocketChatAccountSettings *settings() const;
    DDPClient *ddp();

    bool editingMode() const;

    DDPClient::LoginStatus loginStatus();
    RocketChatRestApi::RestApiRequest *restApi();

    //Make it private in future
    void slotInformTypingStatus(const QString &room, bool typing);

    MessageQueue *messageQueue() const;

    RocketChatBackend *rocketChatBackend() const;

    RuqolaLogger *ruqolaLogger() const;

    void loadEmoji(const QJsonObject &obj);
    void parsePublicSettings(const QJsonObject &obj);

    RuqolaServerConfig *ruqolaServerConfig() const;

    void setUserName(const QString &username);
    Q_REQUIRED_RESULT QString userName() const;

    void setAccountName(const QString &servername);
    Q_REQUIRED_RESULT QString accountName() const;

    void setUserID(const QString &userID);
    Q_REQUIRED_RESULT QString userID() const;

    void setPassword(const QString &password);
    Q_REQUIRED_RESULT QString password() const;

    void setAuthToken(const QString &token);
    Q_REQUIRED_RESULT QString authToken() const;

    void setServerUrl(const QString &serverUrl);

    void sendNotification(const QJsonArray &contents);
    void parseOtr(const QJsonArray &contents);
    void setServerVersion(const QString &version);

    Q_REQUIRED_RESULT bool needAdaptNewSubscriptionRC60() const;
    EmojiManager *emojiManager() const;
    Q_REQUIRED_RESULT QString userStatusIconFileName(const QString &id);

    void membersInRoom(const QString &roomId, const QString &roomType);
    void parseUsersForRooms(const QJsonObject &obj, const QString &roomId);

    void loadAutoCompleteChannel(const QJsonObject &obj);

    void insertCompleterUsers();

    void inputChannelAutocomplete(const QString &pattern, const QString &exceptions);
    void inputUserAutocomplete(const QString &pattern, const QString &exceptions);
    void inputTextCompleter(const QJsonObject &obj);
    PluginAuthenticationInterface *defaultAuthenticationInterface() const;

    SearchMessageModel *searchMessageModel() const;

    void displaySearchedMessage(const QJsonObject &obj);

    void updateUser(const QJsonObject &object);

    void removeSettings();

    void rolesChanged(const QJsonArray &contents);

    Q_INVOKABLE EmoticonModel *emoticonModel() const;
    void setEmoticonModel(EmoticonModel *emoticonModel);

    FilesForRoomModel *filesModelForRoom() const;

Q_SIGNALS:
    void connectedChanged();
    void accountNameChanged();
    void userNameChanged();
    void userIDChanged();
    void passwordChanged();
    void serverUrlChanged();
    void loginStatusChanged();
    void logoutDone(const QString &accountname);
    void added(const QJsonObject &item);
    void changed(const QJsonObject &item);
    void removed(const QJsonObject &item);
    void notification(const QString &title, const QString &message, const QPixmap &pixmap);
    void fileDownloaded(const QString &filePath, const QUrl &cacheImageUrl);
    void updateNotification(bool hasAlert, int nbUnread, const QString &accountName);
    void missingChannelPassword(const QString &roomId);
    void editingModeChanged();
    void jobFailed(const QString &message);
private:
    Q_DISABLE_COPY(RocketChatAccount)
    void slotChannelFilesDone(const QJsonObject &obj, const QString &roomId);
    void slotChannelRolesDone(const QJsonObject &obj, const QString &roomId);
    void slotSplotLightDone(const QJsonObject &obj);
    void slotGetThreadMessagesDone(const QJsonObject &obj, const QString &threadMessageId);
    void slotGetThreadsListDone(const QJsonObject &obj, const QString &roomId);
    void slotGetDiscussionsListDone(const QJsonObject &obj, const QString &roomId);
    void slotGetAllUserMentionsDone(const QJsonObject &obj, const QString &roomId);

    void loadEmojiRestApi(const QJsonObject &obj);
    void slotSearchMessages(const QJsonObject &obj);
    void slotNeedToUpdateNotification();
    void insertAvatarUrl(const QString &userId, const QString &url);
    void loadSettings(const QString &accountFileName);
    void clearModels();
    void fillOauthModel();
    void initializeAuthenticationPlugins();
    void setDefaultAuthentication(AuthenticationManager::OauthType type);
    void userStatusChanged(const User &user);
    void setChannelJoinDone(const QString &roomId);
    void openArchivedRoom(const QString &roomId);

    void checkInitializedRoom(const QString &roomId);
    void clearTypingNotification();

    QVector<File> parseFilesInChannel(const QJsonObject &obj);

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

    SearchMessageModel *mSearchMessageModel = nullptr;
    SearchMessageFilterProxyModel *mSearchMessageFilterProxyModel = nullptr;
    ReceiveTypingNotificationManager *mReceiveTypingNotificationManager = nullptr;
    ServerConfigInfo *mServerConfigInfo = nullptr;
    FilesForRoomModel *mFilesModelForRoom = nullptr;
    FilesForRoomFilterProxyModel *mFilesForRoomFilterProxyModel = nullptr;
    EmoticonModel *mEmoticonModel = nullptr;
    bool mEditingMode = false;
};

#endif // ROCKETCHATACCOUNT_H
