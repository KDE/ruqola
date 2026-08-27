/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QSharedData>
#include <memory>
class QJsonObject;

class QDebug;
class LIBRUQOLACORE_EXPORT MessageEncrypted : public QSharedData
{
public:
    MessageEncrypted();
    ~MessageEncrypted();
    explicit MessageEncrypted(const MessageEncrypted &other);

    [[nodiscard]] bool operator==(const MessageEncrypted &other) const;
    void parse(const QJsonObject &o);

    // Read a "rc.v1.aes-sha2" payload: a bare "kid + base64(iv + ciphertext)" string instead of the
    // {kid, iv, ciphertext} object of the current format. It is normalised into the same fields.
    [[nodiscard]] bool parseLegacyPayload(const QString &prefixedCiphertext);

    [[nodiscard]] static QJsonObject serialize(const MessageEncrypted &message);
    [[nodiscard]] static std::unique_ptr<MessageEncrypted> deserialize(const QJsonObject &o);

    [[nodiscard]] bool isValid() const;

    [[nodiscard]] QByteArray algorithm() const;
    void setAlgorithm(const QByteArray &newAlgorithm);

    [[nodiscard]] QString ciphertext() const;
    void setCiphertext(const QString &newCiphertext);

    [[nodiscard]] QByteArray keyId() const;
    void setKeyId(const QByteArray &newKeyId);

    [[nodiscard]] QByteArray iv() const;
    void setIv(const QByteArray &newIv);

    [[nodiscard]] QByteArray decrypt(const QByteArray &sessionKey) const;
    [[nodiscard]] bool encrypt(const QByteArray &plainText, const QByteArray &sessionKey, const QByteArray &keyId = {});

    void decryptContent(const QByteArray &sessionKey) const;
    [[nodiscard]] QByteArray decryptedContent() const;

    [[nodiscard]] QString descriptedText() const;

    [[nodiscard]] bool hasDescriptedContent() const;

private:
    LIBRUQOLACORE_NO_EXPORT void parseDecryptedContent() const;
    QByteArray mAlgorithm;
    QByteArray mKeyId;
    QString mCiphertext; // TODO QByteArray ?
    QByteArray mIv;
    mutable QByteArray mDecryptedContent;
    mutable QString mDescriptedText;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const MessageEncrypted &t);
