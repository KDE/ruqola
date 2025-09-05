/*
  SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once
#include "libruqola_private_export.h"
#include <QString>
extern "C" {
#include <openssl/bn.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
}
namespace EncryptionUtils
{
struct LIBRUQOLACORE_TESTS_EXPORT EncryptionInfo {
    QByteArray vector;
    QByteArray encryptedData;
    [[nodiscard]] bool isValid() const;
    [[nodiscard]] bool operator==(const EncryptionInfo &other) const;
};

[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QByteArray exportJWKKey(RSA *rsaKey);
LIBRUQOLACORE_TESTS_EXPORT void generateRSAKey();
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QString encodePrivateKey(const QString &privateKey, const QString &password, const QString &userId);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QByteArray getMasterKey(const QString &password, const QString &userId);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QByteArray encryptAES_CBC(const QByteArray &data, const QByteArray &key, const QByteArray &iv);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QByteArray deriveKey(const QByteArray &salt, const QByteArray &baseKey, int iterations = 1000, int keyLength = 32);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QByteArray generateRandomIV(int size);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QString generateRandomText(int size);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT EncryptionUtils::EncryptionInfo splitVectorAndEcryptedData(const QByteArray &cipherText);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QByteArray joinVectorAndEcryptedData(const EncryptionUtils::EncryptionInfo &info);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QVector<uint8_t> toArrayBuffer(const QByteArray &ba);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QByteArray importRawKey(const QByteArray &keyData, const QByteArray &salt, int iterations);
LIBRUQOLACORE_TESTS_EXPORT void importRSAKey();
LIBRUQOLACORE_TESTS_EXPORT void importAESKey();
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QString generateRandomPassword();
};
Q_DECLARE_TYPEINFO(EncryptionUtils::EncryptionInfo, Q_RELOCATABLE_TYPE);
