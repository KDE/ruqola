/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomencryptionkey.h"
#include "ruqola_encryption_debug.h"
#include "ruqola_room_memory_debug.h"
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUuid>
#include <QVector>
#if USE_E2E_SUPPORT
#include "encryption/encryptionutils.h"
#endif

namespace
{
QByteArray decodeBase64Variants(const QByteArray &text)
{
    if (text.isEmpty()) {
        return {};
    }

    // Prefer URL-safe decoding when URL-safe alphabet is present.
    const bool looksBase64Url = text.contains('-') || text.contains('_');
    QByteArray first = QByteArray::fromBase64(text, looksBase64Url ? QByteArray::Base64UrlEncoding : QByteArray::Base64Encoding);
    QByteArray second = QByteArray::fromBase64(text, looksBase64Url ? QByteArray::Base64Encoding : QByteArray::Base64UrlEncoding);

    if (!first.isEmpty() && !second.isEmpty()) {
        // Keep the longest candidate to avoid truncated decodes.
        return (first.size() >= second.size()) ? first : second;
    }
    if (!first.isEmpty()) {
        return first;
    }
    return second;
}

#if USE_E2E_SUPPORT
QVector<QByteArray> decodeAllBase64Variants(const QString &text)
{
    QVector<QByteArray> out;
    const QByteArray bytes = text.toLatin1();

    QByteArray plain = QByteArray::fromBase64(bytes, QByteArray::Base64Encoding);
    if (!plain.isEmpty()) {
        out.append(std::move(plain));
    }

    QByteArray url = QByteArray::fromBase64(bytes, QByteArray::Base64UrlEncoding);
    if (!url.isEmpty() && !out.contains(url)) {
        out.append(std::move(url));
    }
    return out;
}

QVector<QByteArray> encryptedKeyCandidates(const QString &fullE2EKey, const QString &selectedPayload, const QString &knownKeyId)
{
    QVector<QByteArray> out;

    auto appendDecoded = [&](const QString &candidateText) {
        if (candidateText.isEmpty()) {
            return;
        }
        const auto decoded = decodeAllBase64Variants(candidateText);
        for (const QByteArray &d : decoded) {
            if (!out.contains(d)) {
                out.append(d);
            }
        }
    };

    appendDecoded(selectedPayload);
    appendDecoded(fullE2EKey);

    // Some payloads are keyId(36) + ciphertext even when keyId is not UUID-shaped
    // or not yet known in this object. Try fixed-length splits as fallbacks.
    if (fullE2EKey.size() > 36) {
        appendDecoded(fullE2EKey.mid(36));
        const QChar possibleSeparator = fullE2EKey.at(36);
        if (possibleSeparator == QLatin1Char(':') || possibleSeparator == QLatin1Char('|') || possibleSeparator == QLatin1Char('.')) {
            appendDecoded(fullE2EKey.mid(37));
        }
    }

    if (!knownKeyId.isEmpty() && fullE2EKey.startsWith(knownKeyId)) {
        const QString suffix = fullE2EKey.mid(knownKeyId.size());
        appendDecoded(suffix);
        if (!suffix.isEmpty() && (suffix.at(0) == QLatin1Char(':') || suffix.at(0) == QLatin1Char('|') || suffix.at(0) == QLatin1Char('.'))) {
            appendDecoded(suffix.mid(1));
        }
    }

    if (fullE2EKey.size() > 36) {
        const QString possibleKeyId = fullE2EKey.left(36);
        if (!QUuid(possibleKeyId).isNull()) {
            appendDecoded(fullE2EKey.mid(36));
        }
    }

    return out;
}

QByteArray normalizeSessionKeyPayload(const QByteArray &decryptedPayload)
{
    if (decryptedPayload.size() == 32) {
        return decryptedPayload;
    }

    const QByteArray trimmed = decryptedPayload.trimmed();
    if (trimmed.isEmpty()) {
        return {};
    }

    // Some payloads are base64/base64url text of the raw 32-byte key.
    if (const QByteArray decoded = decodeBase64Variants(trimmed); decoded.size() == 32) {
        return decoded;
    }

    // Some Rocket.Chat payloads are JSON (JWK-like), containing the key in "k".
    const QJsonDocument doc = QJsonDocument::fromJson(trimmed);
    if (!doc.isNull()) {
        if (doc.isObject()) {
            const QJsonObject obj = doc.object();
            if (const QString k = obj.value(QStringLiteral("k")).toString(); !k.isEmpty()) {
                const QByteArray decodedK = QByteArray::fromBase64(k.toLatin1(), QByteArray::Base64UrlEncoding);
                if (decodedK.size() == 32) {
                    return decodedK;
                }
            }
            if (const QString key = obj.value(QStringLiteral("key")).toString(); !key.isEmpty()) {
                if (const QByteArray decodedKey = decodeBase64Variants(key.toLatin1()); decodedKey.size() == 32) {
                    return decodedKey;
                }
            }
            if (const QString binary = obj.value(QStringLiteral("$binary")).toString(); !binary.isEmpty()) {
                const QByteArray decodedBinary = QByteArray::fromBase64(binary.toLatin1());
                if (decodedBinary.size() == 32) {
                    return decodedBinary;
                }
            }
        }
    }

    return {};
}
#endif
}

RoomEncryptionKey::RoomEncryptionKey()
{
    qCDebug(RUQOLA_ROOM_MEMORY_LOG) << " RoomEncryptionKey created " << this;
}

RoomEncryptionKey::~RoomEncryptionKey()
{
    qCDebug(RUQOLA_ROOM_MEMORY_LOG) << " RoomEncryptionKey deleted " << this;
}

QString RoomEncryptionKey::e2EKey() const
{
    return mE2EKey;
}

void RoomEncryptionKey::setE2EKey(const QString &newE2EKey)
{
    if (mE2EKey != newE2EKey) {
        mE2EKey = newE2EKey;
        parseSessionKey();
    }
}

void RoomEncryptionKey::parseSessionKey()
{
    if (mE2EKey.isEmpty()) {
        mSessionKey.clear();
        mE2eKeyId.clear();
        return;
    }
    // Rocket.Chat payloads may be either:
    // 1) keyId(36 UUID) + encryptedKey(base64/base64url)
    // 2) encryptedKey(base64/base64url) only, with keyId in a separate field.
    const QByteArray fullCandidate = decodeBase64Variants(mE2EKey.toLatin1());
    QByteArray tailCandidate;
    QString prefixedKeyId;
    QString prefixedCipherText;

    if (!mE2eKeyId.isEmpty() && mE2EKey.startsWith(mE2eKeyId)) {
        prefixedKeyId = mE2eKeyId;
        prefixedCipherText = mE2EKey.mid(mE2eKeyId.size());
        if (!prefixedCipherText.isEmpty()
            && (prefixedCipherText.at(0) == QLatin1Char(':') || prefixedCipherText.at(0) == QLatin1Char('|') || prefixedCipherText.at(0) == QLatin1Char('.'))) {
            prefixedCipherText = prefixedCipherText.mid(1);
        }
        tailCandidate = decodeBase64Variants(prefixedCipherText.toLatin1());
    } else if (mE2EKey.size() > 36) {
        const QString possibleKeyId = mE2EKey.left(36);
        if (!QUuid(possibleKeyId).isNull()) {
            prefixedKeyId = possibleKeyId;
            prefixedCipherText = mE2EKey.mid(36);
            tailCandidate = decodeBase64Variants(prefixedCipherText.toLatin1());
        }
    }

    bool useFullPayload = false;
    if (!tailCandidate.isEmpty()) {
        // When a prefixed form is detected, it is usually the canonical payload.
        useFullPayload = false;
    } else if (!fullCandidate.isEmpty()) {
        useFullPayload = true;
    } else {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Failed to decode E2EKey from base64/base64url";
        mSessionKey.clear();
        return;
    }
    if (useFullPayload) {
        mEncryptedKeyBase64 = mE2EKey;
    } else {
        if (mE2eKeyId.isEmpty()) {
            mE2eKeyId = std::move(prefixedKeyId);
        }
        mEncryptedKeyBase64 = std::move(prefixedCipherText);
    }

    // Validate encoded payload can be decoded as base64/base64url.
    const QByteArray encryptedKey = decodeBase64Variants(mEncryptedKeyBase64.toLatin1());
    if (encryptedKey.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Failed to decode E2EKey from base64/base64url";
        mSessionKey.clear();
        return;
    }
    const QByteArray keyFingerprint = QCryptographicHash::hash(encryptedKey, QCryptographicHash::Sha256).toHex().left(16);
    qCDebug(RUQOLA_ENCRYPTION_LOG) << "E2EKey parsed candidate" << "keyId=" << mE2eKeyId << "base64Len=" << mEncryptedKeyBase64.size()
                                   << "decodedLen=" << encryptedKey.size() << "format=" << (useFullPayload ? "full" : "prefixed")
                                   << "sha256[:16]=" << keyFingerprint;

    qCDebug(RUQOLA_ENCRYPTION_LOG) << "E2EKey parsed - keyId:" << mE2eKeyId << "encryptedKey size:" << encryptedKey.size();
    // Waiting for RSA private key to decrypt session key
}

QString RoomEncryptionKey::e2ESuggestedKey() const
{
    return mE2ESuggestedKey;
}

void RoomEncryptionKey::setE2ESuggestedKey(const QString &newE2ESuggestedKey)
{
    mE2ESuggestedKey = newE2ESuggestedKey;
}

QString RoomEncryptionKey::e2eKeyId() const
{
    return mE2eKeyId;
}

void RoomEncryptionKey::setE2eKeyId(const QString &newE2eKeyId)
{
    mE2eKeyId = newE2eKeyId;
}
#if USE_E2E_SUPPORT
void RoomEncryptionKey::decryptWithPrivateKey(RSA *privateKey)
{
    qCDebug(RUQOLA_ENCRYPTION_LOG) << "RoomEncryptionKey::decryptWithPrivateKey start" << "keyId=" << mE2eKeyId;
    if (!privateKey) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Private key is null, cannot decrypt session key";
        mSessionKey.clear();
        return;
    }

    if (mEncryptedKeyBase64.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "No encrypted key available for decryption";
        mSessionKey.clear();
        return;
    }

    const int rsaSize = RSA_size(privateKey);
    const QVector<QByteArray> keyCandidates = encryptedKeyCandidates(mE2EKey, mEncryptedKeyBase64, mE2eKeyId);
    if (keyCandidates.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Failed to decode encrypted key from base64/base64url";
        mSessionKey.clear();
        return;
    }

    QByteArray lastDecryptedPayload;
    QVector<int> candidateSizes;
    candidateSizes.reserve(keyCandidates.size());

    for (int i = 0; i < keyCandidates.size(); ++i) {
        const QByteArray &encryptedKey = keyCandidates.at(i);
        candidateSizes.append(encryptedKey.size());

        const QByteArray keyFingerprint = QCryptographicHash::hash(encryptedKey, QCryptographicHash::Sha256).toHex().left(16);
        qCDebug(RUQOLA_ENCRYPTION_LOG) << "Decrypting room session key"
                                       << "keyId=" << mE2eKeyId << "candidate=" << i << "candidateCount=" << keyCandidates.size()
                                       << "base64Len=" << mEncryptedKeyBase64.size() << "decodedLen=" << encryptedKey.size() << "rsaSize=" << rsaSize
                                       << "sha256[:16]=" << keyFingerprint;

        if (encryptedKey.size() != rsaSize) {
            continue;
        }

        // Decrypt using RSA private key.
        const QByteArray decryptedPayload = EncryptionUtils::decryptSessionKey(encryptedKey, privateKey);
        if (decryptedPayload.isEmpty()) {
            continue;
        }

        lastDecryptedPayload = decryptedPayload;
        mSessionKey = normalizeSessionKeyPayload(decryptedPayload);
        if (mSessionKey.size() == 32) {
            qCDebug(RUQOLA_ENCRYPTION_LOG) << "Session key successfully decrypted for keyId:" << mE2eKeyId;
            return;
        }
    }

    if (lastDecryptedPayload.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Invalid encryptedKey size candidates:" << candidateSizes << "(expected" << rsaSize
                                         << "for current RSA private key)";
    } else {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Invalid decrypted session key size:" << mSessionKey.size()
                                         << "(expected 32), payloadLen=" << lastDecryptedPayload.size()
                                         << "payloadPreview=" << QString::fromLatin1(lastDecryptedPayload.left(48));
    }
    mSessionKey.clear();
}
#endif

bool RoomEncryptionKey::operator==(const RoomEncryptionKey &other) const
{
    return other.mE2EKey == mE2EKey && other.mE2eKeyId == mE2eKeyId;
}

QList<QByteArray> RoomEncryptionKey::usersWaitingForE2EKeys() const
{
    return mUsersWaitingForE2EKeys;
}

void RoomEncryptionKey::setUsersWaitingForE2EKeys(const QList<QByteArray> &newUsersWaitingForE2EKeys)
{
    mUsersWaitingForE2EKeys = newUsersWaitingForE2EKeys;
}

QByteArray RoomEncryptionKey::sessionKey() const
{
    return mSessionKey;
}
