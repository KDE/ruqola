/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

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
class QNetworkAccessManager;
class QNetworkReply;
class RestApiRequest : public QObject
{
    Q_OBJECT
public:
    enum RestMethod {
        Unknown = 0,
        Login,
        Logout,
        ChannelList
    };
    Q_ENUM(RestMethod)

    explicit RestApiRequest(QObject *parent = nullptr);
    ~RestApiRequest();


    QString userId() const;
    QString authToken() const;


    //Assign/get server url
    QString serverUrl() const;
    void setServerUrl(const QString &serverUrl);

    void setUserName(const QString &userName);
    void setPassword(const QString &password);

    void login();
    void logout();
    void channelList();
private:

    void slotResult(QNetworkReply *reply);
    void slotSslErrors(QNetworkReply *reply, const QList<QSslError> &error);
    void parseLogin(const QByteArray &data);
    QNetworkAccessManager *mNetworkAccessManager = nullptr;
    QString mUserId;
    QString mAuthToken;
    QString mServerUrl;
    QString mUserName;
    QString mPassword;
    bool mSearchRoom = false;
};

#endif // RESTAPIREQUEST_H
