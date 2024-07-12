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
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QByteArray exportJWKKey(RSA *rsaKey);
LIBRUQOLACORE_TESTS_EXPORT void generateRSAKey();
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QString encodePrivateKey(const QString &privateKey, const QString &password);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QString deriveKey();
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QString getMasterKey(const QString &password);

void importRawKey();
};
