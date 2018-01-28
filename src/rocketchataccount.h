/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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
#include <ddpapi/ddpclient.h>
#include "rocketchataccountsettings.h"
#include "libruqola_private_export.h"
class TypingNotification;
class UsersModel;
class RoomModel;
class RoomWrapper;
class MessageModel;
class DDPClient;
class RestApiRequest;
class MessageQueue;
class RocketChatBackend;
class UsersForRoomModel;
class RoomFilterProxyModel;
class RuqolaLogger;
class RuqolaServerConfig;
class UserCompleterModel;
class StatusModel;
class RocketChatCache;
class EmojiManager;

class LIBRUQOLACORE_TESTS_EXPORT RocketChatAccount : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString userID READ userID WRITE setUserID NOTIFY userIDChanged)
    Q_PROPERTY(QString serverUrl READ serverUrl WRITE setServerUrl NOTIFY serverUrlChanged)
    Q_PROPERTY(QString accountName READ accountName WRITE setAccountName NOTIFY accountNameChanged)
    Q_PROPERTY(QString password WRITE setPassword)
    Q_PROPERTY(DDPClient::LoginStatus loginStatus READ loginStatus NOTIFY loginStatusChanged)
public:
    explicit RocketChatAccount(const QString &accountName = QString(), QObject *parent = nullptr);
    ~RocketChatAccount();

    enum RoomInfoType {
        Announcement,
        Description,
        Name,
        Topic,
        ReadOnly,
        Archive
    };
    Q_ENUM(RoomInfoType)

    RocketChatAccountSettings *settings() const;

    Q_INVOKABLE UsersModel *usersModel() const;

    Q_INVOKABLE RoomModel *roomModel() const;

    Q_INVOKABLE RoomFilterProxyModel *roomFilterProxyModel() const;
    Q_INVOKABLE RoomWrapper *getRoom(const QString &roomId);
    Q_INVOKABLE MessageModel *getMessageModelForRoom(const QString &roomID);
    Q_INVOKABLE UsersForRoomModel *getUsersForRoomModel(const QString &roomId);
    Q_INVOKABLE QString getUserCurrentMessage(const QString &roomId);
    Q_INVOKABLE void setUserCurrentMessage(const QString &message, const QString &roomId);

    Q_INVOKABLE void textEditing(const QString &roomId, const QString &str);

    Q_INVOKABLE void leaveRoom(const QString &roomId);
    Q_INVOKABLE void hideRoom(const QString &roomId);
    Q_INVOKABLE void tryLogin();
    Q_INVOKABLE void logOut();
    Q_INVOKABLE void clearUnreadMessages(const QString &roomId);

    Q_INVOKABLE void changeFavorite(const QString &roomId, bool checked);

    DDPClient *ddp();

    DDPClient::LoginStatus loginStatus();
    RestApiRequest *restApi();

    //Make it private in future
    void slotInformTypingStatus(const QString &room, bool typing);
    Q_INVOKABLE void sendMessage(const QString &roomID, const QString &message);
    Q_INVOKABLE void updateMessage(const QString &roomID, const QString &messageId, const QString &message);

    MessageQueue *messageQueue() const;

    RocketChatBackend *rocketChatBackend() const;

    RuqolaLogger *ruqolaLogger() const;

    Q_INVOKABLE void openChannel(const QString &url);
    Q_INVOKABLE void joinJitsiConfCall(const QString &roomId);
    Q_INVOKABLE void createNewChannel(const QString &name, bool readOnly, bool privateRoom, const QString &userNames);
    Q_INVOKABLE void joinRoom(const QString &roomId, const QString &joinCode = QString());
    Q_INVOKABLE void openDirectChannel(const QString &username);
    Q_INVOKABLE void listEmojiCustom();
    Q_INVOKABLE void setDefaultStatus(User::PresenceStatus status);
    Q_INVOKABLE void changeDefaultStatus(int index);
    Q_INVOKABLE void createJitsiConfCall(const QString &roomId);
    Q_INVOKABLE void deleteMessage(const QString &messageId);
    Q_INVOKABLE void userAutocomplete(const QString &searchText, const QString &exception);
    Q_INVOKABLE void eraseRoom(const QString &roomId);
    Q_INVOKABLE void changeChannelSettings(const QString &roomId, RocketChatAccount::RoomInfoType infoType, const QVariant &newValue);
    Q_INVOKABLE QString recordingVideoPath() const;
    Q_INVOKABLE QString recordingImagePath() const;
    Q_INVOKABLE void downloadFile(const QString &downloadFileUrl, const QUrl &localFile);
    Q_INVOKABLE void starMessage(const QString &messageId, const QString &rid, bool starred);
    Q_INVOKABLE void uploadFile(const QString &description, const QUrl &fileUrl);

    void loadEmoji(const QJsonObject &obj);
    void parsePublicSettings(const QJsonObject &obj);

    RuqolaServerConfig *ruqolaServerConfig() const;

    void setUserName(const QString &username);
    QString userName() const;

    void setAccountName(const QString &servername);
    QString accountName() const;

    void setUserID(const QString &userID);
    QString userID() const;

    void setPassword(const QString &password);
    QString password() const;

    void setAuthToken(const QString &token);
    QString authToken() const;

    Q_INVOKABLE QString serverUrl() const;
    void setServerUrl(const QString &serverUrl);

    UserCompleterModel *userCompleterModel() const;

    Q_INVOKABLE QString avatarUrl(const QString &userId);

    Q_INVOKABLE StatusModel *statusModel() const;

    Q_INVOKABLE QUrl attachmentUrl(const QString &url);
    Q_INVOKABLE void loadHistory(const QString &roomID, bool initial = false);
    Q_INVOKABLE bool allowEditingMessages() const;
    Q_INVOKABLE bool otrEnabled() const;

    void sendNotification(const QJsonArray &contents);
    void parseOtr(const QJsonArray &contents);
    void setServerVersion(const QString &version);

Q_SIGNALS:
    void accountNameChanged();
    void userNameChanged();
    void userIDChanged();
    void serverUrlChanged();
    void loginStatusChanged();
    void added(const QJsonObject &item);
    void changed(const QJsonObject &item);
    void notification(const QString &title, const QString &message, const QPixmap &pixmap);
    void fileDownloaded(const QString &filePath, const QUrl &cacheImageUrl);
    void updateNotification(bool hasAlert, int nbUnread, const QString &accountName);

private:
    Q_DISABLE_COPY(RocketChatAccount)
    void slotNeedToUpdateNotification();
    void insertAvatarUrl(const QString &userId, const QString &url);
    void loadSettings();
    void clearModels();
    RocketChatAccountSettings *mSettings = nullptr;
    //room, messagemodel
    QHash<QString, MessageModel *> mMessageModels;
    QHash<QString, UsersForRoomModel *> mUsersForRoomModels;
    QHash<QString, QString> mUserCurrentMessage;

    EmojiManager *mEmojiManager = nullptr;
    TypingNotification *mTypingNotification = nullptr;
    UsersModel *mUserModel = nullptr;
    RoomModel *mRoomModel = nullptr;
    RoomFilterProxyModel *mRoomFilterProxyModel = nullptr;
    DDPClient *mDdp = nullptr;
    RestApiRequest *mRestApi = nullptr;
    MessageQueue *mMessageQueue = nullptr;
    RocketChatBackend *mRocketChatBackend = nullptr;
    RuqolaLogger *mRuqolaLogger = nullptr;
    RuqolaServerConfig *mRuqolaServerConfig = nullptr;
    UserCompleterModel *mUserCompleterModel = nullptr;
    StatusModel *mStatusModel = nullptr;
    RocketChatCache *mCache = nullptr;
};

#endif // ROCKETCHATACCOUNT_H
