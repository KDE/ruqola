/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>
  SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once
#include "libruqolacore_export.h"
#include <QString>
extern "C" {
#include <openssl/bn.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
}

namespace EncryptionUtils
{
struct LIBRUQOLACORE_EXPORT EncryptionInfo {
    QByteArray vector;
    QByteArray encryptedData;
    [[nodiscard]] bool isValid() const;
    [[nodiscard]] bool operator==(const EncryptionInfo &other) const;
};
struct RSAKeyPair {
    QByteArray publicKey;
    QByteArray privateKey;
};

[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray exportJWKPublicKey(RSA *rsaKey);
[[nodiscard]] LIBRUQOLACORE_EXPORT RSAKeyPair generateRSAKey();
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray exportJWKEncryptedPrivateKey(const QByteArray &encryptedPrivateKey);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray encryptPrivateKey(const QByteArray &privateKey, const QByteArray &masterKey);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray decryptPrivateKey(const QByteArray &encryptedPrivateKey, const QByteArray &masterKey);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray getMasterKey(const QString &password, const QString &userId);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray encryptAES_CBC_256(const QByteArray &data, const QByteArray &key, const QByteArray &iv);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray decryptAES_CBC_256(const QByteArray &data, const QByteArray &key, const QByteArray &iv);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray encryptAES_CBC_128(const QByteArray &data, const QByteArray &key, const QByteArray &iv);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray decryptAES_CBC_128(const QByteArray &data, const QByteArray &key, const QByteArray &iv);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray encryptMessage(const QByteArray &plainText, const QByteArray &sessionKey);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray decryptMessage(const QByteArray &plainText, const QByteArray &sessionKey);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray deriveKey(const QByteArray &salt, const QByteArray &baseKey, int iterations = 1000, int keyLength = 32);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray generateRandomIV(int size);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray generateSessionKey();
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray encryptSessionKey(const QByteArray &sessionKey, RSA *publicKey);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray decryptSessionKey(const QByteArray &encryptedSessionKey, RSA *privateKey);
[[nodiscard]] LIBRUQOLACORE_EXPORT RSA *publicKeyFromPEM(const QByteArray &pem);
[[nodiscard]] LIBRUQOLACORE_EXPORT RSA *privateKeyFromPEM(const QByteArray &pem);
[[nodiscard]] LIBRUQOLACORE_EXPORT QString generateRandomText(int size);
[[nodiscard]] LIBRUQOLACORE_EXPORT EncryptionUtils::EncryptionInfo splitVectorAndEcryptedData(const QByteArray &cipherText);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray joinVectorAndEcryptedData(const EncryptionUtils::EncryptionInfo &info);
[[nodiscard]] LIBRUQOLACORE_EXPORT QVector<uint8_t> toArrayBuffer(const QByteArray &ba);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray importRawKey(const QByteArray &keyData, const QByteArray &salt, int iterations);
LIBRUQOLACORE_EXPORT void importRSAKey();
LIBRUQOLACORE_EXPORT void importAESKey();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString generateRandomPassword();
};
Q_DECLARE_TYPEINFO(EncryptionUtils::EncryptionInfo, Q_RELOCATABLE_TYPE);
