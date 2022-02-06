/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

    bool privateChannel = false;

    Q_REQUIRED_RESULT QJsonDocument json(bool createTeam) const;
};
}
Q_DECLARE_TYPEINFO(RocketChatRestApi::CreateRoomInfo, Q_MOVABLE_TYPE);
LIBROCKETCHATRESTAPI_QT5_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::CreateRoomInfo &t);
