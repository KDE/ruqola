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
// Serialise a private key the way WebCrypto's exportKey("jwk") does: this is the plaintext
// Rocket.Chat clients expect to find once they decrypted the stored private key.
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray exportJWKPrivateKey(RSA *rsaKey);
[[nodiscard]] LIBRUQOLACORE_EXPORT RSAKeyPair generateRSAKey();
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray exportJWKEncryptedPrivateKey(const QByteArray &encryptedPrivateKey);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray encryptPrivateKey(const QByteArray &privateKey, const QByteArray &masterKey);
// Encrypt a private key in the "V2" layout every Rocket.Chat client can read back:
// {"iv":…, "ciphertext":…, "salt":…, "iterations":…} (PBKDF2-SHA256 + AES-GCM-256).
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray encryptPrivateKeyV2(const QByteArray &privateKey, const QString &password, const QString &userId);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray decryptPrivateKey(const QByteArray &encryptedPrivateKey, const QByteArray &masterKey);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray getMasterKey(const QString &password, const QString &userId);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray encryptAES_CBC_256(const QByteArray &data, const QByteArray &key, const QByteArray &iv);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray decryptAES_CBC_256(const QByteArray &data, const QByteArray &key, const QByteArray &iv);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray encryptAES_CBC_128(const QByteArray &data, const QByteArray &key, const QByteArray &iv);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray decryptAES_CBC_128(const QByteArray &data, const QByteArray &key, const QByteArray &iv);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray decryptAES_GCM_256(const QByteArray &ciphertext, const QByteArray &key, const QByteArray &iv);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray encryptAES_GCM_256(const QByteArray &plainText, const QByteArray &key, const QByteArray &iv);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray privateKeyJWKToPEM(const QByteArray &jwkJson);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray publicKeyJWKToPEM(const QByteArray &jwkJson);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray encryptMessage(const QByteArray &plainText, const QByteArray &sessionKey);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray decryptMessage(const QByteArray &plainText, const QByteArray &sessionKey);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray deriveKey(const QByteArray &salt, const QByteArray &baseKey, int iterations = 1000, int keyLength = 32);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray generateRandomIV(int size);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray generateSessionKey();
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray sessionKeyToJWK(const QByteArray &rawKey);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray encryptSessionKey(const QByteArray &sessionKey, RSA *publicKey);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray decryptSessionKey(const QByteArray &encryptedSessionKey, RSA *privateKey);
// Caller owns the returned RSA object and must release it with RSA_free().
[[nodiscard]] LIBRUQOLACORE_EXPORT RSA *publicKeyFromPEM(const QByteArray &pem);
// Caller owns the returned RSA object and must release it with RSA_free().
[[nodiscard]] LIBRUQOLACORE_EXPORT RSA *privateKeyFromPEM(const QByteArray &pem);
[[nodiscard]] LIBRUQOLACORE_EXPORT QString generateRandomText(int size);
[[nodiscard]] LIBRUQOLACORE_EXPORT EncryptionUtils::EncryptionInfo splitVectorAndEcryptedData(const QByteArray &cipherText);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray joinVectorAndEcryptedData(const EncryptionUtils::EncryptionInfo &info);
[[nodiscard]] LIBRUQOLACORE_EXPORT QVector<uint8_t> toArrayBuffer(const QByteArray &ba);
[[nodiscard]] LIBRUQOLACORE_EXPORT QString generateRandomPassword();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString generateRoomKeyId();
}
Q_DECLARE_TYPEINFO(EncryptionUtils::EncryptionInfo, Q_RELOCATABLE_TYPE);
