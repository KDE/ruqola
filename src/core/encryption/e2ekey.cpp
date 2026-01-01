/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2ekey.h"

#include <QJsonObject>

QT_IMPL_METATYPE_EXTERN_TAGGED(E2eKey, Ruqola_E2eKey)
using namespace Qt::Literals::StringLiterals;

E2eKey::E2eKey() = default;

E2eKey::~E2eKey() = default;

void E2eKey::parseKeyInfo(const QJsonObject &replyObject)
{
    const QJsonObject publicKey = replyObject["public_key"_L1].toObject();
    // TODO split ? publicKey.
    const QJsonObject privateKey = replyObject["private_key"_L1].toObject();
    setBinaryPrivateKey(privateKey["$binary"_L1].toString());
}

QString E2eKey::binaryPrivateKey() const
{
    return mBinaryPrivateKey;
}

void E2eKey::setBinaryPrivateKey(const QString &newBinaryPrivateKey)
{
    mBinaryPrivateKey = newBinaryPrivateKey;
}

QString E2eKey::publicKey() const
{
    return mPublicKey;
}

void E2eKey::setPublicKey(const QString &newPublicKey)
{
    mPublicKey = newPublicKey;
}

QDebug operator<<(QDebug d, const E2eKey &t)
{
    d.space() << "mBinaryPrivateKey" << t.binaryPrivateKey();
    d.space() << "mPublicKey" << t.publicKey();
    return d;
}
