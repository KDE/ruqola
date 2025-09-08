/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "loginmanager.h"

LoginManager::LoginManager(QObject *parent)
    : QObject(parent)
{
}

void LoginManager::login(const QString &serverUrl, QNetworkAccessManager *networkManager, int userIndex)
{
    const auto envPath = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(QStringLiteral("../../.env"));
    const auto creds = loadEnvFile(envPath);

    const auto usernameKey = userIndex == 0 ? QStringLiteral("USERNAME") : QStringLiteral("USERNAME%1").arg(userIndex);
    const auto passwordKey = userIndex == 0 ? QStringLiteral("PASSWORD") : QStringLiteral("PASSWORD%1").arg(userIndex);

    loginJob = new RocketChatRestApi::LoginJob(this);
    restApiMethod = new RocketChatRestApi::RestApiMethod();
    restApiMethod->setServerUrl(serverUrl);

    loginJob->setRestApiMethod(restApiMethod);
    loginJob->setNetworkAccessManager(networkManager);

    if (creds.value(usernameKey).isEmpty() || creds.value(passwordKey).isEmpty()) {
        qDebug() << "Username or password are empty!";
    }

    loginJob->setUserName(creds.value(usernameKey));
    loginJob->setPassword(creds.value(passwordKey));

    QObject::connect(loginJob, &RocketChatRestApi::LoginJob::loginDone, this, [this](const QString &authToken, const QString &userId) {
        Q_EMIT loginSucceeded(authToken, userId);
    });

    QObject::connect(loginJob, &RocketChatRestApi::RestApiAbstractJob::failed, this, [this](const QString &err) {
        Q_EMIT loginFailed(err);
    });

    loginJob->start();
}
