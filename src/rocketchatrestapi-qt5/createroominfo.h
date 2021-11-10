/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include <QDebug>
#include <QJsonDocument>
#include <QString>
#include <QStringList>
namespace RocketChatRestApi
{
struct LIBROCKETCHATRESTAPI_QT5_EXPORT CreateRoomInfo {
    Q_REQUIRED_RESULT bool isValid() const;
    Q_REQUIRED_RESULT bool canStart() const;
    QString name;
    QString description;
    QString teamId;

    QStringList members;
    bool readOnly = false;
    bool broadcast = false;
    bool encrypted = false;

    Q_REQUIRED_RESULT QJsonDocument json() const;
};
}
Q_DECLARE_TYPEINFO(RocketChatRestApi::CreateRoomInfo, Q_MOVABLE_TYPE);
LIBROCKETCHATRESTAPI_QT5_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::CreateRoomInfo &t);
