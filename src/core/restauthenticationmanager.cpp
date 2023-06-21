/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

#include "moc_restauthenticationmanager.cpp"
