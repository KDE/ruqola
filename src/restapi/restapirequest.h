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

#ifndef RESTAPIREQUEST_H
#define RESTAPIREQUEST_H

#include <QObject>
#include <QSslError>
#include <QUrl>
#include "restapiutil.h"

class QNetworkAccessManager;
class QNetworkReply;
class QNetworkCookieJar;
class RestApiMethod;
class RuqolaLogger;
class RestApiAbstractJob;
class RestApiRequest : public QObject
{
    Q_OBJECT
public:
    explicit RestApiRequest(QObject *parent = nullptr);
    ~RestApiRequest();

    void setRuqolaLogger(RuqolaLogger *logger);

    QString userId() const;
    QString authToken() const;
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
    void createChannels(const QString &channelName, bool readOnly, const QStringList &members);
    void createGroups(const QString &channelName, bool readOnly, const QStringList &members);
    void leaveChannel(const QString &roomId);
    void leaveGroups(const QString &roomId);
    void updateMessage(const QString &roomId, const QString &messageId, const QString &text);
    void reactOnMessage(const QString &messageId, const QString &emoji);
    void closeChannel(const QString &roomId, const QString &type);
    void createDirectMessage(const QString &userName);
    void historyChannel(const QString &roomId, const QString &type);
    void changeChannelDescription(const QString &roomId, const QString &description);
    void changeGroupsDescription(const QString &roomId, const QString &description);
    void archiveChannel(const QString &roomId);
    void archiveGroups(const QString &roomId);
    void filesInRoom(const QString &roomId, const QString &type);
    void inviteInChannel(const QString &roomId, const QString &userId);

    void listEmojiCustom();
    void searchRoomUser(const QString &pattern);
Q_SIGNALS:
    void avatar(const QString &userId, const QString &url);
    void logoutDone();
    void loginDone(const QString &authToken, const QString &userId);
    void getDataDone(const QByteArray &data, const QUrl &url, bool useCache, const QUrl &localFileUrl);
    void getServerInfoDone(const QString &version);
    void getOwnInfoDone(const QJsonObject &data);
    void privateInfoDone(const QByteArray &data);
    void channelFilesDone(const QJsonObject &obj);
    void loadEmojiCustomDone(const QJsonObject &obj);
    void spotlightDone(const QJsonObject &obj);
    void channelListDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(RestApiRequest)
    void initializeCookies();
    void slotResult(QNetworkReply *reply);
    void slotSslErrors(QNetworkReply *reply, const QList<QSslError> &error);
    void slotLogout();
    void slotLogin(const QString &authToken, const QString &userId);

    void initializeRestApiJob(RestApiAbstractJob *job, bool needAuthentication);
    QNetworkAccessManager *mNetworkAccessManager = nullptr;
    QNetworkCookieJar *mCookieJar = nullptr;
    RestApiMethod *mRestApiMethod = nullptr;
    RuqolaLogger *mRuqolaLogger = nullptr;
    QString mUserId;
    QString mAuthToken;
    QString mUserName;
    QString mPassword;
};

#endif // RESTAPIREQUEST_H
