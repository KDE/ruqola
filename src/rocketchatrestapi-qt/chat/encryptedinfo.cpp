/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "encryptedinfo.h"
#include <QJsonObject>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
QJsonObject EncryptedInfo::generateJson() const
{
    QJsonObject o;
    o["algorithm"_L1] = QString::fromLatin1(algorithm);
    o["ciphertext"_L1] = ciphertext;
    o["iv"_L1] = QString::fromLatin1(iv);
    o["kid"_L1] = QString::fromLatin1(keyId);
    return o;
}

bool EncryptedInfo::isValid() const
{
    return !algorithm.isEmpty() && !keyId.isEmpty() && !ciphertext.isEmpty() && !iv.isEmpty();
}
