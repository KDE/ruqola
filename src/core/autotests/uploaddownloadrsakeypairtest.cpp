/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploaddownloadrsakeypair.h"
#include "loginmanager.h"
#include "uploaddownloadrsakeypairtest.h"
#include <QCoreApplication>
#include <QDebug>
#include <QObject>
#include <QTest>

QTEST_GUILESS_MAIN(UploadDownloadRsaKeyPairTest)

void UploadDownloadRsaKeyPairTest::uploadDownloadCompare()
{
    const auto app = QCoreApplication::instance();
    const auto loginManager = new LoginManager(app);
    const auto networkManager = new QNetworkAccessManager(app);
    const auto url = QStringLiteral("http://localhost:3000");
    const auto password = QStringLiteral("mypassword123");
    bool testPassed = false;

    QObject::connect(loginManager, &LoginManager::loginSucceeded, this, [=, &testPassed](const QString &authToken, const QString &userId) {
        qDebug() << "Login succeeded! authToken:" << authToken << "userId:" << userId;
        uploadKeys(authToken, url, userId, password, networkManager, [=, &testPassed](const QString &message, const EncryptionUtils::RSAKeyPair &keypair) {
            downloadKeys(authToken, url, userId, password, networkManager, [=, &testPassed](const QString &publicKey, const QString &decryptedPrivateKey) {
                QCOMPARE(publicKey, QString::fromUtf8(keypair.publicKey));
                QCOMPARE(decryptedPrivateKey, QString::fromUtf8(keypair.privateKey));
                testPassed = true;
                app->quit();
            });
        });
    });

    QObject::connect(loginManager, &LoginManager::loginFailed, this, [&](const QString &err) {
        QFAIL(qPrintable(QStringLiteral("Login failed: %1").arg(err)));
        app->quit();
    });

    loginManager->login(url, networkManager, 0);
    app->exec();

    QVERIFY(testPassed);
}

#include "uploaddownloadrsakeypairtest.moc"
