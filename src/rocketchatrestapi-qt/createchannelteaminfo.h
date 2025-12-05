/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "librocketchatrestapi-qt_export.h"
#include <QJsonDocument>
#include <QMetaType>
#include <QString>
#include <QStringList>
namespace RocketChatRestApi
{
struct LIBROCKETCHATRESTAPI_QT_EXPORT CreateChannelTeamInfo {
    Q_GADGET
public:
    enum class CreateInfoType : uint8_t {
        Unknown = 0,
        Channel = 1,
        Team = 2,
    };
    Q_ENUM(CreateInfoType)

    [[nodiscard]] bool isValid() const;
    [[nodiscard]] bool canStart() const;
    QString name;
    QString topic;
    QByteArray teamId;

    QStringList members;
    bool readOnly = false;
    bool broadcast = false;
    bool encrypted = false;

    bool privateChannel = false;
    bool federated = false;
    CreateInfoType infoType = CreateInfoType::Unknown;

    [[nodiscard]] QJsonDocument json() const;
};
}
Q_DECLARE_TYPEINFO(RocketChatRestApi::CreateChannelTeamInfo, Q_RELOCATABLE_TYPE);
class QDebug;
LIBROCKETCHATRESTAPI_QT_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::CreateChannelTeamInfo &t);
