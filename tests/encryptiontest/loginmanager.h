/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "authentication/loginjob.h"
#include "envutils.h"
#include "restapimethod.h"
#include <QCoreApplication>
#include <QDir>
#include <QNetworkAccessManager>
#include <QObject>

/**
 * @class LoginManager
 * @brief Utility class for logging into Rocket.Chat as a test user.
 *
 * LoginManager simplifies the process of authenticating test users in integration and autotests.
 * It reads user credentials from a `.env` file located at the project root, supporting multiple users
 * by using environment variable suffixes (e.g., USERNAME1, PASSWORD1, USERNAME2, PASSWORD2, etc.).
 *
 * Usage:
 *
 *   LoginManager lm;
 *   lm.login(serverUrl, networkManager, index);
 *
 *
 *   LoginManager lm2;
 *   lm2.login(serverUrl, networkManager, 2); // Uses USERNAME2, PASSWORD2
 *
 * .env file:
 *
 *   The .env file should be placed at the project root and contain lines like:
 *     USERNAME=alice
 *     PASSWORD=alicepass
 *
 *     USERNAME1=bob
 *     PASSWORD1=bobpass
 *
 *     USERNAME2=carol
 *     PASSWORD2=carolpass
 *
 * This allows you to run tests with multiple users by specifying their credentials.
 *
 **/
class LoginManager : public QObject
{
    Q_OBJECT
public:
    explicit LoginManager(QObject *parent = nullptr);
    void login(const QString &serverUrl, QNetworkAccessManager *networkManager, int userIndex);

Q_SIGNALS:
    void loginSucceeded(const QString &authToken, const QString &userId);
    void loginFailed(const QString &error);

private:
    RocketChatRestApi::LoginJob *loginJob = nullptr;
    RocketChatRestApi::RestApiMethod *restApiMethod = nullptr;
};
