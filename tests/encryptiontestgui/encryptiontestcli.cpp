/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
   SPDX-License-Identifier: LGPL-2.0-or-later
*/

/*

test my existence...of course within server, otherwise I still exist :)

curl -X POST http://localhost:3000/api/v1/login \
  -H "Content-Type: application/json" \
  -d '{"user": "", "password": ""}'
*/

#include "encryption/encryptionutils.h"
#include "loginmanager.h"
#include "uploaddownloadrsakeypair.h"
#include <QCoreApplication>
#include <QDebug>

using namespace EncryptionUtils;

const auto url = u"http://localhost:3000"_s;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    auto networkManager = new QNetworkAccessManager(&app);
    auto loginManager = new LoginManager(&app);
    loginManager->login(url, networkManager, 1);

    QObject::connect(loginManager, &LoginManager::loginSucceeded, &app, [=](const QString &authToken, const QString &userId) {
        qDebug() << "Login successful! Auth token:" << authToken << "UserId:" << userId << "\n";

        uploadKeys(
            authToken,
            url,
            userId,
            u"mypassword123"_s,
            networkManager,
            [authToken, userId, networkManager](const QString &message, const RSAKeyPair &keypair) {
                qDebug() << message;
                qDebug() << keypair.publicKey << keypair.privateKey;

                downloadKeys(authToken, url, userId, u"mypassword123"_s, networkManager, [](const QString &publicKey, const QString &decryptedPrivateKey) {
                    qDebug() << "Downloaded Public Key:" << publicKey;
                    qDebug() << "Decrypted Private Key:" << decryptedPrivateKey;
                    QCoreApplication::quit();
                });
            });
    });

    QObject::connect(loginManager, &LoginManager::loginFailed, &app, [](const QString &err) {
        qCritical() << "Login failed:" << err;
        QCoreApplication::quit();
    });

    return app.exec();
}
