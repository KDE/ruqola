/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "encryptionutils.h"
// https://docs.rocket.chat/customer-center/security-center/end-to-end-encryption-specifications

EncryptionUtils::EncryptionUtils(QObject *parent)
    : QObject{parent}
{
}

EncryptionUtils::~EncryptionUtils() = default;

void EncryptionUtils::generateGlobalKey()
{
    // TODO
    // this RSA-OAEP key pair is generated using a function call to: crypto.subtle.generateKey({name: 'RSA-OAEP', modulusLength: 2048, publicExponent: new
    // Uint8Array([0x01, 0x00, 0x01]), hash: {name: 'SHA-256'}}, true, ['encrypt', 'decrypt']);
}

void EncryptionUtils::decodeMessage()
{
    // TODO
}

#include "moc_encryptionutils.cpp"
