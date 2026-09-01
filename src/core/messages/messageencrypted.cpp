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

    // A "rc.v1.aes-sha2" payload was normalised into the same fields when it was parsed, so both
    // versions decrypt identically from here: what differs is the mode, and that comes from the
    // room key rather than from the content version.
    if (mAlgorithm != "rc.v2.aes-sha2"_ba && mAlgorithm != "rc.v1.aes-sha2"_ba) {
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

    // Rocket.Chat takes the AES mode from the room key itself (Aes.decrypt() passes
    // key.algorithm.name), not from the content version: a room still keyed with a legacy
    // 16-byte A128CBC key produces AES-CBC-128 payloads inside a "rc.v2.aes-sha2" content.
    if (sessionKey.size() == 16) {
        return EncryptionUtils::decryptAES_CBC_128(decodedCiphertext, sessionKey, decodedIv);
    }
    if (sessionKey.size() != 32) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "MessageEncrypted::decrypt: unexpected session key size" << sessionKey.size();
        return {};
    }
    const QByteArray decrypted = EncryptionUtils::decryptAES_GCM_256(decodedCiphertext, sessionKey, decodedIv);
    if (decrypted.isEmpty()) {
        // The room key we were handed is not the one this message was written with. Rocket.Chat
        // looks the message "kid" up in the subscription's oldRoomKeys before falling back to the
        // current key, so a mismatch here usually means the room key was reset after this message.
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "MessageEncrypted::decrypt: cannot decrypt message encrypted with kid" << mKeyId
                                         << "- the current room key does not match it";
    }
    return decrypted;
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

    // Same rule as in decrypt(): the room key decides the mode. Rocket.Chat's Aes.encrypt() also
    // derives the IV length from it, 12 bytes for AES-GCM and 16 for the legacy AES-CBC-128.
    const bool useLegacyCbc = sessionKey.size() == 16;
    if (!useLegacyCbc && sessionKey.size() != 32) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "MessageEncrypted::encrypt: unexpected session key size" << sessionKey.size();
        return false;
    }

    const QByteArray generatedIv = EncryptionUtils::generateRandomIV(useLegacyCbc ? 16 : 12);
    if (generatedIv.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "MessageEncrypted::encrypt: failed to generate iv";
        return false;
    }

    const QByteArray encryptedPayload = useLegacyCbc ? EncryptionUtils::encryptAES_CBC_128(plainText, sessionKey, generatedIv)
                                                     : EncryptionUtils::encryptAES_GCM_256(plainText, sessionKey, generatedIv);
    if (encryptedPayload.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "MessageEncrypted::encrypt: encryption failed";
        return false;
    }

    mAlgorithm = "rc.v2.aes-sha2"_ba;
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
    return mAlgorithm == other.mAlgorithm && mCiphertext == other.mCiphertext && mKeyId == other.mKeyId && mIv == other.mIv;
}

bool MessageEncrypted::parseLegacyPayload(const QString &prefixedCiphertext)
{
    // Port of Rocket.Chat's decodeV1EncryptedContent(): the whole payload is
    // "kid(12 chars) + base64(iv[16] + ciphertext)". Storing it in the fields of the current format
    // keeps a single decryption path and lets the key id be looked up the same way.
    constexpr qsizetype legacyKeyIdLength = 12;
    constexpr qsizetype legacyIvLength = 16;
    if (prefixedCiphertext.size() <= legacyKeyIdLength) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "MessageEncrypted::parseLegacyPayload: too short to carry a key id";
        return false;
    }
    const QByteArray decoded = QByteArray::fromBase64(prefixedCiphertext.mid(legacyKeyIdLength).toLatin1());
    if (decoded.size() <= legacyIvLength) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "MessageEncrypted::parseLegacyPayload: too short to carry an iv and a ciphertext";
        return false;
    }

    mAlgorithm = "rc.v1.aes-sha2"_ba;
    mKeyId = prefixedCiphertext.left(legacyKeyIdLength).toLatin1();
    mIv = decoded.left(legacyIvLength).toBase64();
    mCiphertext = QString::fromLatin1(decoded.mid(legacyIvLength).toBase64());
    return true;
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
