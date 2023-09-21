/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

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
struct LIBROCKETCHATRESTAPI_QT5_EXPORT CreateChannelTeamInfo {
    Q_GADGET
public:
    enum CreateInfoType {
        Unknown = 0,
        Channel = 1,
        Team = 2,
    };
    Q_ENUM(CreateInfoType)

    [[nodiscard]] bool isValid() const;
    [[nodiscard]] bool canStart() const;
    QString name;
    QString description;
    QString teamId;

    QStringList members;
    bool readOnly = false;
    bool broadcast = false;
    bool encrypted = false;

    bool privateChannel = false;
    CreateInfoType infoType = CreateInfoType::Unknown;

    [[nodiscard]] QJsonDocument json() const;
};
}
Q_DECLARE_TYPEINFO(RocketChatRestApi::CreateChannelTeamInfo, Q_MOVABLE_TYPE);
LIBROCKETCHATRESTAPI_QT5_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::CreateChannelTeamInfo &t);
