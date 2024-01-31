/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"

#include <QDebug>
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

    enum OtrType {
        Unknown = 0,
        End,
        Handshake,
        Deny,
        AcknowLedge,
    };
    Q_ENUM(OtrType)

    void parseOtr(const QJsonArray &contents);

    [[nodiscard]] OtrType type() const;

    [[nodiscard]] QString roomId() const;

    [[nodiscard]] QString userId() const;

    [[nodiscard]] bool isValid() const;

    [[nodiscard]] CryptoSettings crypto() const;

    [[nodiscard]] bool operator==(const Otr &other) const;

private:
    LIBRUQOLACORE_NO_EXPORT void parseCryptoSettings(const QString &publicKey);
    LIBRUQOLACORE_NO_EXPORT void extractRoomUser(const QJsonObject &obj);
    // TODO add crypto support
    QString mRoomId;
    QString mUserId;
    OtrType mType = OtrType::Unknown;
    CryptoSettings mCrypto;
};
Q_DECLARE_TYPEINFO(Otr, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Otr &t);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const CryptoSettings &t);
