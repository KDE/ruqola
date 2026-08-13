/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageencrypted.h"
#include "config-ruqola.h"
#include "encryption/encryptionutils.h"
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
    mAlgorithm = other.algorithm();
    mKeyId = other.keyId();
    mCiphertext = other.ciphertext();
    mIv = other.iv();
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

void MessageEncrypted::decryptContent(const QByteArray &sessionKey) const
{
    mDecryptedContent = decrypt(sessionKey);
    parseDecryptedContent();
}

QByteArray MessageEncrypted::decryptedContent() const
{
    return mDecryptedContent;
}

void MessageEncrypted::parseDecryptedContent() const
{
    if (!mDecryptedContent.isEmpty()) {
        const QJsonDocument doc = QJsonDocument::fromJson(mDecryptedContent);
        const QJsonObject obj = doc.object();
        if (obj.contains("msg"_L1)) {
            mDescriptedText = obj["msg"_L1].toString();
        }
    }
}

QString MessageEncrypted::descriptedText() const
{
    return mDescriptedText;
}

QByteArray MessageEncrypted::decrypt([[maybe_unused]] const QByteArray &sessionKey) const
{
#if USE_E2E_SUPPORT
    if (sessionKey.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "MessageEncrypted::decrypt: session key is empty";
        return {};
    }

    if (mAlgorithm != "rc.v2.aes-sha2") {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "MessageEncrypted::decrypt: unsupported algorithm" << mAlgorithm;
        return {};
    }

    const QByteArray decodedIv = QByteArray::fromBase64(mIv);
    if (decodedIv.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "MessageEncrypted::decrypt: iv is invalid";
        return {};
    }

    const QByteArray decodedCiphertext = QByteArray::fromBase64(mCiphertext.toLatin1());
    if (decodedCiphertext.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "MessageEncrypted::decrypt: ciphertext is invalid";
        return {};
    }

    return EncryptionUtils::decryptAES_GCM_256(decodedCiphertext, sessionKey, decodedIv);
#else
    return {};
#endif
}

bool MessageEncrypted::encrypt([[maybe_unused]] const QByteArray &plainText,
                               [[maybe_unused]] const QByteArray &sessionKey,
                               [[maybe_unused]] const QByteArray &keyId)
{
#if USE_E2E_SUPPORT
    if (plainText.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "MessageEncrypted::encrypt: plaintext is empty";
        return false;
    }

    if (sessionKey.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "MessageEncrypted::encrypt: session key is empty";
        return false;
    }

    const QByteArray effectiveKeyId = keyId.isEmpty() ? mKeyId : keyId;
    if (effectiveKeyId.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "MessageEncrypted::encrypt: key id is empty";
        return false;
    }

    const QByteArray generatedIv = EncryptionUtils::generateRandomIV(12);
    if (generatedIv.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "MessageEncrypted::encrypt: failed to generate iv";
        return false;
    }

    const QByteArray encryptedPayload = EncryptionUtils::encryptAES_GCM_256(plainText, sessionKey, generatedIv);
    if (encryptedPayload.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "MessageEncrypted::encrypt: encryption failed";
        return false;
    }

    mAlgorithm = "rc.v2.aes-sha2";
    mKeyId = effectiveKeyId;
    mIv = generatedIv.toBase64();
    mCiphertext = QString::fromLatin1(encryptedPayload.toBase64());
    return true;
#else
    return false;
#endif
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

QJsonObject MessageEncrypted::serialize(const MessageEncrypted &message)
{
    QJsonObject o;
    o["algorithm"_L1] = QString::fromLatin1(message.algorithm());
    o["ciphertext"_L1] = message.ciphertext();
    o["iv"_L1] = QString::fromLatin1(message.iv());
    o["kid"_L1] = QString::fromLatin1(message.keyId());
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

std::unique_ptr<MessageEncrypted> MessageEncrypted::deserialize(const QJsonObject &o)
{
    auto encrypted = std::make_unique<MessageEncrypted>();
    encrypted->setAlgorithm(o["algorithm"_L1].toString().toLatin1());
    encrypted->setCiphertext(o["ciphertext"_L1].toString());
    encrypted->setIv(o["iv"_L1].toString().toLatin1());
    encrypted->setKeyId(o["kid"_L1].toString().toLatin1());
    return encrypted;
}

bool MessageEncrypted::hasDescriptedContent() const
{
    return !mDecryptedContent.isEmpty();
}
