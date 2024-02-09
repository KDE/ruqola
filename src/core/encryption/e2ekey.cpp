/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2ekey.h"

E2eKey::E2eKey()
{
}

E2eKey::~E2eKey() = default;

void E2eKey::parseKeyInfo(const QJsonObject &replyObject)
{
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
