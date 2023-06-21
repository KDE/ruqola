/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "otr.h"
#include "ruqola_debug.h"
#include <QJsonObject>

Otr::Otr() = default;

Otr::~Otr() = default;

void Otr::extractRoomUser(const QJsonObject &obj)
{
    mRoomId = obj.value(QLatin1String("roomId")).toString();
    mUserId = obj.value(QLatin1String("userId")).toString();
}

void Otr::parseOtr(const QJsonArray &contents)
{
    // QJsonArray(["handshake",{"publicKey":"{\"crv\":\"P-256\",\"ext\":true,\"key_ops\":[],\"kty\":\"EC\",\"x\":\"R9TKy7SvVpbJurHngvOICZ5oBHvLt_P19RiBX7-ChBs\",\"y\":\"Ama4y0Sk5DWFRAImF8_4u--qKknOa44EP5hr0VXuEvM\"}","roomId":"4faACeGzSvG7xMcTyYbwG4T2uB3wZSZSKB","userId":"YbwG4T2uB3wZSZSKB"}])
    qCDebug(RUQOLA_LOG) << " contents " << contents;
    const QString type = contents.at(0).toString();
    if (type == QLatin1String("end")) {
        const QJsonObject obj = contents.at(1).toObject();
        extractRoomUser(obj);
        qCDebug(RUQOLA_LOG) << " END" << obj << " roomId " << mRoomId << " userId " << mUserId;
        mType = Otr::End;
    } else if (type == QLatin1String("handshake")) {
        // qDebug() << " HANDSHAKE" << contents.at(1).toObject();
        const QJsonObject obj = contents.at(1).toObject();
        extractRoomUser(obj);
        const QString publicKey = obj.value(QLatin1String("publicKey")).toString();
        qCDebug(RUQOLA_LOG) << " HANDSHAKE" << obj << " roomId " << mRoomId << " userId " << mUserId << " publicKey " << publicKey;
        mType = Otr::Handshake;
        parseCryptoSettings(publicKey);
    } else if (type == QLatin1String("deny")) {
        qCDebug(RUQOLA_LOG) << " Deny " << contents;
        const QJsonObject obj = contents.at(1).toObject();
        extractRoomUser(obj);
        mType = Otr::Deny;
    } else if (type == QLatin1String("acknowledge")) {
        qCDebug(RUQOLA_LOG) << " acknowledge " << contents;
        const QJsonObject obj = contents.at(1).toObject();
        extractRoomUser(obj);
        const QString publicKey = obj.value(QLatin1String("publicKey")).toString();
        parseCryptoSettings(publicKey);
        mType = Otr::AcknowLedge;
    } else {
        qCDebug(RUQOLA_LOG) << " unknown" << type;
    }
}

void Otr::parseCryptoSettings(const QString &publicKey)
{
    qDebug() << " parseCryptoSettings " << publicKey;
    mCrypto.mCrypt = publicKey;

    // TODO parse it.
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

QDebug operator<<(QDebug d, const Otr &t)
{
    d << "isValid: " << t.isValid();
    d << "type : " << t.type();
    d << "userId: " << t.userId();
    d << "roomId: " << t.roomId();
    d << "crypto settings " << t.crypto();
    return d;
}

QDebug operator<<(QDebug d, const CryptoSettings &t)
{
    d << "mCrypt: " << t.mCrypt;
    return d;
}

bool Otr::operator==(const Otr &other) const
{
    return roomId() == other.roomId() && userId() == other.userId() && type() == other.type() && crypto() == other.crypto();
}

bool CryptoSettings::operator==(const CryptoSettings &other) const
{
    return mCrypt == other.mCrypt;
}

#include "moc_otr.cpp"
