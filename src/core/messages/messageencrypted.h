/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QSharedData>
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

    [[nodiscard]] static QJsonObject serialize(const MessageEncrypted &message);
    [[nodiscard]] static MessageEncrypted *deserialize(const QJsonObject &o);

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

private:
    QByteArray mAlgorithm;
    QByteArray mKeyId;
    QString mCiphertext; // TODO QByteArray ?
    QByteArray mIv;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const MessageEncrypted &t);
