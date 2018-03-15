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
#include "restapiutil.h"

class QNetworkAccessManager;
class QNetworkReply;
class QNetworkCookieJar;
class RestApiMethod;
class RestApiRequest : public QObject
{
    Q_OBJECT
public:
    enum RestMethod {
        Unknown = 0,
        Login,
        Logout,
        ChannelList,
        GetAvatar,
        Get,
        Post,
        ServerInfo,
        PrivateInfo,
        Me,
        UploadFile,
    };
    Q_ENUM(RestMethod)

    explicit RestApiRequest(QObject *parent = nullptr);
    ~RestApiRequest();

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

    void post(const QUrl &url, const QByteArray &data, const QString &mimeType = QStringLiteral("text/plain"));
    QNetworkReply *get(const QUrl &url, const QString &mimeType = QStringLiteral("text/plain"));

    void serverInfo();
    void getPrivateSettings();
    void getOwnInfo();
    void uploadFile(const QString &roomId, const QString &description, const QString &text, const QUrl &filename);
Q_SIGNALS:
    void avatar(const QString &userId, const QString &url);
    void logoutDone();
    void loginDone(const QString &authToken, const QString &userId);
    void getDataDone(const QByteArray &data, const QUrl &url, bool useCache, const QUrl &localFileUrl);
    void getServerInfoDone(const QString &version);
    void getOwnInfoDone(const QByteArray &data);

private:
    Q_DISABLE_COPY(RestApiRequest)
    void initializeCookies();
    void slotResult(QNetworkReply *reply);
    void slotSslErrors(QNetworkReply *reply, const QList<QSslError> &error);

    void parsePrivateInfo(const QByteArray &data);
    void parseServerInfo(const QByteArray &data);
    void parseGetAvatar(const QByteArray &data, const QString &userId);
    void parseLogout(const QByteArray &data);
    void parseLogin(const QByteArray &data);
    void parseOwnInfo(const QByteArray &data);
    void parseChannelList(const QByteArray &data);
    void parseGet(const QByteArray &data, const QUrl &url, bool storeInCache, const QUrl &localFile);
    void parsePost(const QByteArray &data);
    void parseUploadFile(const QByteArray &data);
    QNetworkAccessManager *mNetworkAccessManager = nullptr;
    QNetworkCookieJar *mCookieJar = nullptr;
    RestApiMethod *mRestApiMethod = nullptr;
    QString mUserId;
    QString mAuthToken;
    QString mUserName;
    QString mPassword;
};

#endif // RESTAPIREQUEST_H
