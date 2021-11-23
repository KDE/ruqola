/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "restauthenticationmanager.h"

#include "connection.h"

RESTAuthenticationManager::RESTAuthenticationManager(RocketChatRestApi::Connection *restApiConnection, QObject *parent)
    : QObject(parent)
    , mRestApiConnection(restApiConnection)
{
}

RESTAuthenticationManager::~RESTAuthenticationManager() = default;

void RESTAuthenticationManager::setAuthToken(const QString &authToken)
{
    mAuthToken = authToken;
}

void RESTAuthenticationManager::login()
{
    mRestApiConnection->login();
}

void RESTAuthenticationManager::login(const QString &user, const QString &password)
{
}

void RESTAuthenticationManager::loginLDAP(const QString &user, const QString &password)
{
}

void RESTAuthenticationManager::loginOAuth(const QString &credentialToken, const QString &credentialSecret)
{
}

void RESTAuthenticationManager::sendOTP(const QString &otp)
{
}

void RESTAuthenticationManager::logout()
{
    mRestApiConnection->logout();
}
