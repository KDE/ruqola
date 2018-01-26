/*
   Copyright (C) 2018 Montel Laurent <montel@kde.org>

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
    qDebug() << " contents " << contents;
    const QString type = contents.at(0).toString();
    if (type == QLatin1String("end")) {
        const QJsonObject obj =  contents.at(1).toObject();
        const QString roomId = obj.value(QLatin1String("roomId")).toString();
        const QString userId = obj.value(QLatin1String("userId")).toString();
        qDebug() << " END" << obj << " roomId " << roomId << " userId " << userId;

        //const QString roomId = contents
    } else if (type == QLatin1String("handshake")) {
        //qDebug() << " HANDSHAKE" << contents.at(1).toObject();
        const QJsonObject obj =  contents.at(1).toObject();
        const QString roomId = obj.value(QLatin1String("roomId")).toString();
        const QString userId = obj.value(QLatin1String("userId")).toString();
        const QString publicKey = obj.value(QLatin1String("publicKey")).toString();
        //TODO parsing publicKey.
        qDebug() << " HANDSHAKE" << obj << " roomId " << roomId << " userId " << userId << " publicKey "<<publicKey;
    } else if (type == QLatin1String("deny")) {
        qDebug() << " Deny " << contents;
    } else if (type == QLatin1String("acknowledge")) {
        qDebug() << " acknowledge " << contents;
    } else {
        qDebug() << " unknown" << type;
    }
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
