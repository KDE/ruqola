/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"

#include <QDebug>
#include <QJsonArray>

struct CryptoSettings {
    QString mCrypt;
    Q_REQUIRED_RESULT bool operator==(const CryptoSettings &other) const;
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

    Q_REQUIRED_RESULT OtrType type() const;

    Q_REQUIRED_RESULT QString roomId() const;

    Q_REQUIRED_RESULT QString userId() const;

    Q_REQUIRED_RESULT bool isValid() const;

    Q_REQUIRED_RESULT CryptoSettings crypto() const;

    Q_REQUIRED_RESULT bool operator==(const Otr &other) const;

private:
    void parseCryptoSettings(const QString &publicKey);
    void extractRoomUser(const QJsonObject &obj);
    // TODO add crypto support
    QString mRoomId;
    QString mUserId;
    OtrType mType = OtrType::Unknown;
    CryptoSettings mCrypto;
};
Q_DECLARE_METATYPE(Otr)
Q_DECLARE_TYPEINFO(Otr, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Otr &t);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const CryptoSettings &t);
