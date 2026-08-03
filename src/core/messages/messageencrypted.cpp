/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageencrypted.h"
#include "ruqola_encryption_debug.h"

#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
MessageEncrypted::MessageEncrypted()

{
    qCDebug(RUQOLA_ENCRYPTION_LOG) << " MessageEncrypted created " << this;
}

MessageEncrypted::MessageEncrypted(const MessageEncrypted &other)
    : QSharedData(other)
{
    qCDebug(RUQOLA_ENCRYPTION_LOG) << " MessageEncrypted created " << this;
}

MessageEncrypted::~MessageEncrypted()
{
    qCDebug(RUQOLA_ENCRYPTION_LOG) << " MessageEncrypted deleted " << this;
}

bool MessageEncrypted::isValid() const
{
    return false;
    // return !mPinnedBy.isEmpty();
}

QByteArray MessageEncrypted::algorithm() const
{
    return mAlgorithm;
}

void MessageEncrypted::setAlgorithm(const QByteArray &newAlgorithm)
{
    mAlgorithm = newAlgorithm;
}

QString MessageEncrypted::ciphertext() const
{
    return mCiphertext;
}

void MessageEncrypted::setCiphertext(const QString &newCiphertext)
{
    mCiphertext = newCiphertext;
}

QByteArray MessageEncrypted::keyId() const
{
    return mKeyId;
}

void MessageEncrypted::setKeyId(const QByteArray &newKeyId)
{
    mKeyId = newKeyId;
}

QByteArray MessageEncrypted::iv() const
{
    return mIv;
}

void MessageEncrypted::setIv(const QByteArray &newIv)
{
    mIv = newIv;
}

bool MessageEncrypted::operator==(const MessageEncrypted &other) const
{
    return mAlgorithm == other.algorithm() && mCiphertext == other.ciphertext() && mKeyId == other.keyId() && mIv == other.iv();
}

void MessageEncrypted::parse(const QJsonObject &o)
{
    // TODO
}

QJsonObject MessageEncrypted::serialize(const MessageEncrypted &messagePinned)
{
    QJsonObject o;
    // TODO
    return o;
}

QDebug operator<<(QDebug d, const MessageEncrypted &t)
{
    d.space() << "algorithm:" << t.algorithm();
    d.space() << "ciphertext:" << t.ciphertext();
    d.space() << "keyId:" << t.keyId();
    d.space() << "iv:" << t.iv();
    return d;
}

MessageEncrypted *MessageEncrypted::deserialize(const QJsonObject &o)
{
    MessageEncrypted *encrypted = new MessageEncrypted;
    // TODO
    return encrypted;
}
