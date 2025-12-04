/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"

#include <QJsonArray>

struct CryptoSettings {
    QString mCrypt;
    [[nodiscard]] bool operator==(const CryptoSettings &other) const;
};

class LIBRUQOLACORE_EXPORT Otr
{
    Q_GADGET
public:
    Otr();
    ~Otr();

    enum class OtrType : uint8_t {
        Unknown = 0,
        End,
        Handshake,
        Deny,
        AcknowLedge,
    };
    Q_ENUM(OtrType)

    void parseOtr(const QJsonArray &contents);

    [[nodiscard]] OtrType type() const;

    [[nodiscard]] QByteArray roomId() const;

    [[nodiscard]] QByteArray userId() const;

    [[nodiscard]] bool isValid() const;

    [[nodiscard]] CryptoSettings crypto() const;

    [[nodiscard]] bool operator==(const Otr &other) const;

private:
    LIBRUQOLACORE_NO_EXPORT void parseCryptoSettings(const QString &publicKey);
    LIBRUQOLACORE_NO_EXPORT void extractRoomUser(const QJsonObject &obj);
    // TODO add crypto support
    QByteArray mRoomId;
    QByteArray mUserId;
    OtrType mType = OtrType::Unknown;
    CryptoSettings mCrypto;
};
Q_DECLARE_TYPEINFO(Otr, Q_RELOCATABLE_TYPE);
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Otr &t);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const CryptoSettings &t);
