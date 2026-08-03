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
    return !mAlgorithm.isEmpty() && !mKeyId.isEmpty() && !mCiphertext.isEmpty() && !mIv.isEmpty();
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
    mAlgorithm = o["algorithm"_L1].toString().toLatin1();
    mCiphertext = o["ciphertext"_L1].toString();
    mIv = o["iv"_L1].toString().toLatin1();
    mKeyId = o["kid"_L1].toString().toLatin1();
}

QJsonObject MessageEncrypted::serialize(const MessageEncrypted &encrypted)
{
    QJsonObject o;
    o["algorithm"_L1] = QString::fromLatin1(encrypted.algorithm());
    o["ciphertext"_L1] = encrypted.ciphertext();
    o["iv"_L1] = QString::fromLatin1(encrypted.iv());
    o["kid"_L1] = QString::fromLatin1(encrypted.keyId());
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
    encrypted->setAlgorithm(o["algorithm"_L1].toString().toLatin1());
    encrypted->setCiphertext(o["ciphertext"_L1].toString());
    encrypted->setCiphertext(o["ciphertext"_L1].toString());
    encrypted->setIv(o["iv"_L1].toString().toLatin1());
    encrypted->setKeyId(o["kid"_L1].toString().toLatin1());
    return encrypted;
}
