/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2ekey.h"
using namespace Qt::Literals::StringLiterals;

E2eKey::E2eKey() = default;

E2eKey::~E2eKey() = default;

void E2eKey::parseKeyInfo(const QJsonObject &replyObject)
{
    const QJsonObject publicKey = replyObject["public_key"_L1].toObject();

    const QJsonObject privateKey = replyObject["private_key"_L1].toObject();
    // TODO
}

QString E2eKey::binaryPrivateKey() const
{
    return mBinaryPrivateKey;
}

void E2eKey::setBinaryPrivateKey(const QString &newBinaryPrivateKey)
{
    mBinaryPrivateKey = newBinaryPrivateKey;
}

QDebug operator<<(QDebug d, const E2eKey &t)
{
    // TODO
    return d;
}
