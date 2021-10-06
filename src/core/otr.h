/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#pragma once

#include "libruqola_private_export.h"
#include <QDebug>
#include <QJsonArray>

struct CryptoSettings {
    QString mCrypt;
    Q_REQUIRED_RESULT bool operator==(const CryptoSettings &other) const;
};

class LIBRUQOLACORE_TESTS_EXPORT Otr
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
    // TODO add crypto support
    QString mRoomId;
    QString mUserId;
    OtrType mType = OtrType::Unknown;
    CryptoSettings mCrypto;
};
Q_DECLARE_METATYPE(Otr)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Otr &t);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const CryptoSettings &t);

