/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef RESTAUTHENTICATIONMANAGER_H
#define RESTAUTHENTICATIONMANAGER_H

#include <QObject>
#include "libruqolacore_export.h"
namespace RocketChatRestApi {
class RestApiRequest;
}
class LIBRUQOLACORE_EXPORT RESTAuthenticationManager : public QObject
{
    Q_OBJECT
public:
    explicit RESTAuthenticationManager(RocketChatRestApi::RestApiRequest *restApiRequest, QObject *parent = nullptr);
    ~RESTAuthenticationManager();

    void login();
    void login(const QString &user, const QString &password);
    void loginLDAP(const QString &user, const QString &password); // TODO: LDAP options?
    void loginOAuth(const QString &credentialToken, const QString &credentialSecret);
    void sendOTP(const QString &otp);
    void logout();

    void setAuthToken(const QString &authToken);
private:
    QString mAuthToken;
    RocketChatRestApi::RestApiRequest *const mRestApiResquest;
};

#endif // RESTAUTHENTICATIONMANAGER_H
