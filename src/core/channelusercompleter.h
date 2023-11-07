/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>

class LIBRUQOLACORE_EXPORT ChannelUserCompleter
{
    Q_GADGET
public:
    ChannelUserCompleter();
    ~ChannelUserCompleter();

    enum ChannelUserCompleterType {
        Room,
        DirectChannel,
        Unknown,
    };
    Q_ENUM(ChannelUserCompleterType)

    void parseChannel(const QJsonObject &object, ChannelUserCompleter::ChannelUserCompleterType);
};
Q_DECLARE_TYPEINFO(ChannelUserCompleter, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ChannelUserCompleter &t);
