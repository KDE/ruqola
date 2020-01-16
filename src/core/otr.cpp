/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#include "otr.h"
#include "ruqola_debug.h"
#include <QDebug>
#include <QJsonObject>

Otr::Otr()
{
}

Otr::~Otr()
{
}

void Otr::parseOtr(const QJsonArray &contents)
{
    qCDebug(RUQOLA_LOG) << " contents " << contents;
    const QString type = contents.at(0).toString();
    if (type == QLatin1String("end")) {
        const QJsonObject obj = contents.at(1).toObject();
        const QString roomId = obj.value(QLatin1String("roomId")).toString();
        const QString userId = obj.value(QLatin1String("userId")).toString();
        qCDebug(RUQOLA_LOG) << " END" << obj << " roomId " << roomId << " userId " << userId;
        mUserId = userId;
        mRoomId = roomId;
        mType = Otr::End;
    } else if (type == QLatin1String("handshake")) {
        //qDebug() << " HANDSHAKE" << contents.at(1).toObject();
        const QJsonObject obj = contents.at(1).toObject();
        const QString roomId = obj.value(QLatin1String("roomId")).toString();
        const QString userId = obj.value(QLatin1String("userId")).toString();
        const QString publicKey = obj.value(QLatin1String("publicKey")).toString();
        qCDebug(RUQOLA_LOG) << " HANDSHAKE" << obj << " roomId " << roomId << " userId " << userId << " publicKey "<<publicKey;
        mUserId = userId;
        mRoomId = roomId;
        mType = Otr::Handshake;
        parseCryptoSettings(publicKey);
    } else if (type == QLatin1String("deny")) {
        qCDebug(RUQOLA_LOG) << " Deny " << contents;
        //TODO
        mType = Otr::Deny;
    } else if (type == QLatin1String("acknowledge")) {
        qCDebug(RUQOLA_LOG) << " acknowledge " << contents;
        const QJsonObject obj = contents.at(1).toObject();
        const QString roomId = obj.value(QLatin1String("roomId")).toString();
        const QString userId = obj.value(QLatin1String("userId")).toString();
        const QString publicKey = obj.value(QLatin1String("publicKey")).toString();
        mUserId = userId;
        mRoomId = roomId;
        parseCryptoSettings(publicKey);
        mType = Otr::AcknowLedge;
    } else {
        qCDebug(RUQOLA_LOG) << " unknown" << type;
    }
}

void Otr::parseCryptoSettings(const QString &publicKey)
{
    //TODO
}

CryptoSettings Otr::crypto() const
{
    return mCrypto;
}

Otr::OtrType Otr::type() const
{
    return mType;
}

QString Otr::roomId() const
{
    return mRoomId;
}

QString Otr::userId() const
{
    return mUserId;
}

bool Otr::isValid() const
{
    return mType != Otr::Unknown;
}

QDebug operator <<(QDebug d, const Otr &t)
{
    d << "isValid: " << t.isValid();
    d << "type : " << t.type();
    d << "userId: " << t.userId();
    d << "roomId: " << t.roomId();
    d << "crypto settings " << t.crypto();
    return d;
}

QDebug operator <<(QDebug d, const CryptoSettings &t)
{
    d << "isValid: " << t.mCrypt;
    return d;
}
