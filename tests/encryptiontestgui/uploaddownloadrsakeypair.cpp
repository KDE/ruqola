/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploaddownloadrsakeypair.h"
#include <QCoreApplication>
#include <QDebug>
#include <QJsonObject>

using namespace EncryptionUtils;
using namespace RocketChatRestApi;

void uploadKeys(const QString &authToken,
                const QString &url,
                const QString &userId,
                const QString &password,
                QNetworkAccessManager *networkManager,
                std::function<void(QString, RSAKeyPair)> onSuccess)
{
    const auto keyPair = generateRSAKey();
    const auto masterKey = getMasterKey(password, u"salt"_s);
    const auto encryptedPrivateKey = encryptPrivateKey(keyPair.privateKey, masterKey);

    qDebug() << "uploadKeys called with authToken:" << authToken;
    const auto uploadJob = new SetUserPublicAndPrivateKeysJob();
    const auto restApiMethod = new RestApiMethod();
    restApiMethod->setServerUrl(url);

    uploadJob->setRestApiMethod(restApiMethod);
    uploadJob->setNetworkAccessManager(networkManager);
    uploadJob->setAuthToken(authToken);
    uploadJob->setUserId(userId);

    SetUserPublicAndPrivateKeysJob::SetUserPublicAndPrivateKeysInfo info;
    info.rsaPublicKey = QString::fromUtf8(keyPair.publicKey);
    info.rsaPrivateKey = QString::fromUtf8(encryptedPrivateKey.toBase64());
    info.force = true;
    uploadJob->setSetUserPublicAndPrivateKeysInfo(info);

    QObject::connect(uploadJob, &SetUserPublicAndPrivateKeysJob::setUserPublicAndPrivateKeysDone, uploadJob, [onSuccess, keyPair]() {
        if (onSuccess) {
            onSuccess(u"Key upload successful!"_s, keyPair);
        }
    });

    QObject::connect(uploadJob, &SetUserPublicAndPrivateKeysJob::failed, uploadJob, [](const QString &err) {
        qCritical() << "Key upload failed!: " << err;
        QCoreApplication::quit();
    });

    uploadJob->start();
}

void downloadKeys(const QString &authToken,
                  const QString &url,
                  const QString &userId,
                  const QString &password,
                  QNetworkAccessManager *networkManager,
                  std::function<void(QString, QString)> onSuccess)
{
    const auto fetchJob = new FetchMyKeysJob();
    const auto restApiMethod = new RestApiMethod();
    restApiMethod->setServerUrl(url);

    fetchJob->setRestApiMethod(restApiMethod);
    fetchJob->setNetworkAccessManager(networkManager);
    fetchJob->setAuthToken(authToken);
    fetchJob->setUserId(userId);

    QObject::connect(fetchJob, &FetchMyKeysJob::fetchMyKeysDone, fetchJob, [password, onSuccess](const QJsonObject &jsonObj) {
        const auto publicKey = jsonObj["public_key"_L1].toString();
        const auto encryptedPrivateKeyB64 = jsonObj["private_key"_L1].toString();
        const auto encryptedPrivateKey = QByteArray::fromBase64(encryptedPrivateKeyB64.toUtf8());
        const auto masterKey = getMasterKey(password, u"salt"_s);
        const auto decryptedPrivateKey = QString::fromUtf8(decryptPrivateKey(encryptedPrivateKey, masterKey));

        if (onSuccess) {
            onSuccess(publicKey, decryptedPrivateKey);
        }
    });

    QObject::connect(fetchJob, &RestApiAbstractJob::failed, fetchJob, [=](const QString &err) {
        qCritical() << "Key fetch failed:" << err;
        QCoreApplication::quit();
    });

    fetchJob->start();
}
