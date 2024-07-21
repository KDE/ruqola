/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once
#include "libruqola_private_export.h"
#include <QString>
extern "C" {
#include <openssl/bn.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
}
namespace EncryptionUtils
{
struct EncryptionInfo {
    QByteArray vector;
    QByteArray encryptedData;
};

[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QByteArray exportJWKKey(RSA *rsaKey);
LIBRUQOLACORE_TESTS_EXPORT void generateRSAKey();
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QString encodePrivateKey(const QString &privateKey, const QString &password);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QString deriveKey();
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QString getMasterKey(const QString &password);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QByteArray encryptAES_CBC(const QByteArray &data, const QByteArray &key, const QByteArray &iv);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QByteArray deriveKey(const QByteArray &keyData, const QByteArray &salt, int iterations);

[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QByteArray importRawKey(const QByteArray &keyData, const QByteArray &salt, int iterations);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT EncryptionUtils::EncryptionInfo splitVectorAndEcryptedData(const QByteArray &cipherText);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QByteArray joinVectorAndEcryptedData(const EncryptionUtils::EncryptionInfo &info);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QVector<uint8_t> toArrayBuffer(const QByteArray &ba);
LIBRUQOLACORE_TESTS_EXPORT void importRSAKey();
LIBRUQOLACORE_TESTS_EXPORT void importAESKey();
};
