/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "loginmanager.h"
#include <QDebug>

LoginManager::LoginManager(QObject *parent)
    : QObject(parent)
{
}

void LoginManager::login(const QString &serverUrl, QNetworkAccessManager *networkManager, int userIndex)
{
    const auto envPath = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(u"../../.env"_s);
    const auto creds = loadEnvFile(envPath);

    const auto usernameKey = userIndex == 0 ? u"USERNAME"_s : u"USERNAME%1"_s.arg(userIndex);
    const auto passwordKey = userIndex == 0 ? u"PASSWORD"_s : u"PASSWORD%1"_s.arg(userIndex);

    mLoginJob = new RocketChatRestApi::LoginJob(this);
    mRestApiMethod = new RocketChatRestApi::RestApiMethod();
    mRestApiMethod->setServerUrl(serverUrl);

    mLoginJob->setRestApiMethod(mRestApiMethod);
    mLoginJob->setNetworkAccessManager(networkManager);

    if (creds.value(usernameKey).isEmpty() || creds.value(passwordKey).isEmpty()) {
        qDebug() << "Username or password are empty!";
    }

    mLoginJob->setUserName(creds.value(usernameKey));
    mLoginJob->setPassword(creds.value(passwordKey));

    QObject::connect(mLoginJob, &RocketChatRestApi::LoginJob::loginDone, this, [this](const QString &authToken, const QString &userId) {
        Q_EMIT loginSucceeded(authToken, userId);
    });

    QObject::connect(mLoginJob, &RocketChatRestApi::RestApiAbstractJob::failed, this, [this](const QString &err) {
        Q_EMIT loginFailed(err);
    });

    if (!mLoginJob->start()) {
        qWarning() << "Impossible to start login job";
        delete mRestApiMethod;
    }
}
