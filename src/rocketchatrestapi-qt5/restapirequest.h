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

#ifndef RESTAPIREQUEST_H
#define RESTAPIREQUEST_H

#include <QObject>
#include <QSslError>
#include <QUrl>
#include "restapiutil.h"
#include "librocketchatrestapi-qt5_export.h"

class QNetworkAccessManager;
class QNetworkReply;
class QNetworkCookieJar;
class RestApiMethod;
namespace RocketChatRestApi {
class RestApiAbstractJob;
class AbstractLogger;
class LIBROCKETCHATRESTAPI_QT5_EXPORT RestApiRequest : public QObject
{
    Q_OBJECT
public:
    explicit RestApiRequest(QObject *parent = nullptr);
    ~RestApiRequest();

    void setRestApiLogger(RocketChatRestApi::AbstractLogger *logger);

    Q_REQUIRED_RESULT QString userId() const;
    Q_REQUIRED_RESULT QString authToken() const;
    void setUserId(const QString &userId);
    void setAuthToken(const QString &authToken);

    //Assign/get server url
    QString serverUrl() const;
    void setServerUrl(const QString &serverUrl);

    void setUserName(const QString &userName);
    void setPassword(const QString &password);

    void login();
    void logout();
    void channelList();
    void getAvatar(const QString &userId);

    void serverInfo();
    void getPrivateSettings();
    void getOwnInfo();
    void starMessage(const QString &messageId, bool starred);
    void uploadFile(const QString &roomId, const QString &description, const QString &text, const QUrl &filename);
    void downloadFile(const QUrl &url, const QString &mimeType = QStringLiteral("text/plain"), bool storeInCache = true, const QUrl &localFileUrl = QUrl());
    void changeChannelTopic(const QString &roomId, const QString &topic);
    void changeGroupsTopic(const QString &roomId, const QString &topic);
    void changeChannelAnnouncement(const QString &roomId, const QString &announcement);
    void changeGroupsAnnouncement(const QString &roomId, const QString &announcement);
    void postMessage(const QString &roomId, const QString &text);
    void deleteMessage(const QString &roomId, const QString &messageId);
    void createChannels(const QString &channelName, bool readOnly, const QStringList &members, const QString &password);
    void createGroups(const QString &channelName, bool readOnly, const QStringList &members);
    void leaveChannel(const QString &roomId);
    void leaveGroups(const QString &roomId);
    void updateMessage(const QString &roomId, const QString &messageId, const QString &text);
    void reactOnMessage(const QString &messageId, const QString &emoji, bool shouldReact);
    void closeChannel(const QString &roomId, const QString &type);
    void createDirectMessage(const QString &userName);
    void historyChannel(const QString &roomId, const QString &type);
    void changeChannelDescription(const QString &roomId, const QString &description);
    void changeGroupsDescription(const QString &roomId, const QString &description);
    void archiveChannel(const QString &roomId, bool archive);
    void archiveGroups(const QString &roomId, bool archive);
    void filesInRoom(const QString &roomId, const QString &type, int offset = 0, int count = 50);
    void addUserInChannel(const QString &roomId, const QString &userId);

    void listEmojiCustom();
    void searchRoomUser(const QString &pattern);
    void searchMessages(const QString &roomId, const QString &pattern);
    void markAsRead(const QString &roomId);
    void getRooms();
    void markAsFavorite(const QString &roomId, bool favorite);
    void addUserInGroup(const QString &roomId, const QString &userId);
    void disableNotifications(const QString &roomId, bool value);
    void hideUnreadStatus(const QString &roomId, bool value);
    void audioNotifications(const QString &roomId, const QString &value);
    void desktopNotifications(const QString &roomId, const QString &value);
    void emailNotifications(const QString &roomId, const QString &value);
    void mobilePushNotifications(const QString &roomId, const QString &value);
    void unreadAlert(const QString &roomId, const QString &value);
    void setAvatar(const QString &avatarUrl);
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
    void listPermissions();
    void listCommands();
    void fetchMyKeys();
    void setJoinCodeChannel(const QString &roomId, const QString &joinCode);
    void channelJoin(const QString &roomId, const QString &joinCode);
    void muteGroupMentions(const QString &roomId, bool value);

    void channelInfo(const QString &roomId);

    void changeChannelName(const QString &roomId, const QString &newName);
    void changeGroupName(const QString &roomId, const QString &newName);
    void groupInfo(const QString &roomId);
    void channelGetAllUserMentions(const QString &roomId);
    void updatejitsiTimeOut(const QString &roomId);
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
    void membersInRoom(const QString &roomId, const QString &type);
    void changeChannelReadOnly(const QString &roomId, bool b);
    void changeGroupsReadOnly(const QString &roomId, bool b);
    void channelAddLeader(const QString &roomId, const QString &userId);
    void channelRemoveLeader(const QString &roomId, const QString &userId);
    void changeChannelEncrypted(const QString &roomId, bool b);
    void changeGroupsEncrypted(const QString &roomId, bool b);
    void pinMessage(const QString &messageId, bool pinned);
    void desktopDurationNotifications(const QString &roomId, int value);
    void desktopSoundNotifications(const QString &roomId, const QString &value);
    void followMessage(const QString &messageId);
    void unFollowMessage(const QString &messageId);
    void createDiscussion(const QString &parentRoomId, const QString &discussionName, const QString &replyMessage, const QString &parentMessageId);
    void getDiscussions(const QString &roomId, int offset = 0, int count = 50);
    void getThreadsList(const QString &roomId, int offset = 0, int count = 50);
    void getThreadMessages(const QString &threadMessageId);
    void sendMessage(const QString &roomId, const QString &text, const QString &messageId = QString(), const QString &threadMessageId = QString());
Q_SIGNALS:
    void avatar(const QString &userId, const QString &url);
    void redownloadAvatar();
    void logoutDone();
    void loginDone(const QString &authToken, const QString &userId);
    void downloadFileDone(const QByteArray &data, const QUrl &url, bool useCache, const QUrl &localFileUrl);
    void getServerInfoDone(const QString &version);
    void getOwnInfoDone(const QJsonObject &data);
    void privateInfoDone(const QByteArray &data);
    void channelFilesDone(const QJsonObject &obj, const QString &roomId);
    void channelMembersDone(const QJsonObject &obj, const QString &roomId);
    void loadEmojiCustomDone(const QJsonObject &obj);
    void spotlightDone(const QJsonObject &obj);
    void channelListDone(const QJsonObject &obj);
    void searchMessageDone(const QJsonObject &obj);
    void getRoomsDone(const QJsonObject &obj);
    void usersInfoDone(const QJsonObject &obj);
    void channelRolesDone(const QJsonObject &obj, const QString &roomId);
    void getUsernameSuggestionDone(const QString &username);
    void getPresenceDone(const QString &presence);
    void listPermissionDone(const QJsonObject &obj);
    void listCommandsDone(const QJsonObject &obj);
    void fetchMyKeysDone();
    void setJoinCodeDone();
    void setChannelJoinDone(const QString &roomId);
    void missingChannelPassword(const QString &roomId);
    void openArchivedRoom(const QString &roomId);
    void channelGetAllUserMentionsDone(const QJsonObject &obj, const QString &roomId);
    void updateJitsiTimeOutDone(const QString &datetime);
    void channelKickUserDone(const QJsonObject &obj);
    void groupKickUserDone(const QJsonObject &obj);
    void addModeratorDone();
    void removeModeratorDone();
    void addLeaderDone();
    void removeLeaderDone();
    void addOwnerDone();
    void removeOwnerDone();
    void deletechannelDone();
    void deleteGroupsDone();
    void pinMessageDone();
    void ignoreUserDone(const QJsonObject &obj, const QString &roomId);
    void followMessageDone();
    void unFollowMessageDone();
    void startDiscussionDone();
    void getDiscussionsDone(const QJsonObject &obj, const QString &roomId);
    void getThreadsDone(const QJsonObject &obj, const QString &roomId);
    void getThreadMessagesDone(const QJsonObject &obj, const QString &threadMessageId);

    void failed(const QString &str);

private:
    Q_DISABLE_COPY(RestApiRequest)
    void initializeCookies();
    void slotResult(QNetworkReply *reply);
    void slotSslErrors(QNetworkReply *reply, const QList<QSslError> &error);
    void slotLogout();
    void slotLogin(const QString &authToken, const QString &userId);
    void slotAddJoinCodeToChannel(const QString &channelId, const QString &password);

    void initializeRestApiJob(RocketChatRestApi::RestApiAbstractJob *job);
    QNetworkAccessManager *mNetworkAccessManager = nullptr;
    QNetworkCookieJar *mCookieJar = nullptr;
    RestApiMethod *mRestApiMethod = nullptr;
    RocketChatRestApi::AbstractLogger *mRuqolaLogger = nullptr;
    QString mUserId;
    QString mAuthToken;
    QString mUserName;
    QString mPassword;
};
}

#endif // RESTAPIREQUEST_H
