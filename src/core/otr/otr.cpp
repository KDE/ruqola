/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "otr.h"

#include "ruqola_debug.h"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;

Otr::Otr() = default;

Otr::~Otr() = default;

void Otr::extractRoomUser(const QJsonObject &obj)
{
    mRoomId = obj.value("roomId"_L1).toString().toLatin1();
    mUserId = obj.value("userId"_L1).toString().toLatin1();
}

void Otr::parseOtr(const QJsonArray &contents)
{
    // QJsonArray(["handshake",{"publicKey":"{\"crv\":\"P-256\",\"ext\":true,\"key_ops\":[],\"kty\":\"EC\",\"x\":\"R9TKy7SvVpbJurHngvOICZ5oBHvLt_P19RiBX7-ChBs\",\"y\":\"Ama4y0Sk5DWFRAImF8_4u--qKknOa44EP5hr0VXuEvM\"}","roomId":"4faACeGzSvG7xMcTyYbwG4T2uB3wZSZSKB","userId":"YbwG4T2uB3wZSZSKB"}])
    qCDebug(RUQOLA_LOG) << " contents " << contents;
    const QString type = contents.at(0).toString();
    if (type == "end"_L1) {
        const QJsonObject obj = contents.at(1).toObject();
        extractRoomUser(obj);
        qCDebug(RUQOLA_LOG) << " END" << obj << " roomId " << mRoomId << " userId " << mUserId;
        mType = Otr::OtrType::End;
    } else if (type == "handshake"_L1) {
        // qDebug() << " HANDSHAKE" << contents.at(1).toObject();
        const QJsonObject obj = contents.at(1).toObject();
        extractRoomUser(obj);
        const QString publicKey = obj.value("publicKey"_L1).toString();
        qCDebug(RUQOLA_LOG) << " HANDSHAKE" << obj << " roomId " << mRoomId << " userId " << mUserId << " publicKey " << publicKey;
        mType = Otr::OtrType::Handshake;
        parseCryptoSettings(publicKey);
    } else if (type == "deny"_L1) {
        qCDebug(RUQOLA_LOG) << " Deny " << contents;
        const QJsonObject obj = contents.at(1).toObject();
        extractRoomUser(obj);
        mType = Otr::OtrType::Deny;
    } else if (type == "acknowledge"_L1) {
        qCDebug(RUQOLA_LOG) << " acknowledge " << contents;
        const QJsonObject obj = contents.at(1).toObject();
        extractRoomUser(obj);
        const QString publicKey = obj.value("publicKey"_L1).toString();
        parseCryptoSettings(publicKey);
        mType = Otr::OtrType::AcknowLedge;
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

QByteArray Otr::roomId() const
{
    return mRoomId;
}

QByteArray Otr::userId() const
{
    return mUserId;
}

bool Otr::isValid() const
{
    return mType != Otr::OtrType::Unknown;
}

QDebug operator<<(QDebug d, const Otr &t)
{
    d.space() << "isValid:" << t.isValid();
    d.space() << "type :" << t.type();
    d.space() << "userId:" << t.userId();
    d.space() << "roomId:" << t.roomId();
    d.space() << "crypto settings" << t.crypto();
    return d;
}

QDebug operator<<(QDebug d, const CryptoSettings &t)
{
    d.space() << "mCrypt:" << t.mCrypt;
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
