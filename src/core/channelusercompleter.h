/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QIcon>

class LIBRUQOLACORE_EXPORT ChannelUserCompleter
{
    Q_GADGET
public:
    ChannelUserCompleter();
    ~ChannelUserCompleter();

    enum ChannelUserCompleterType {
        Unknown,
        Room,
        DirectChannel,
    };
    Q_ENUM(ChannelUserCompleterType)

    void parseChannel(const QJsonObject &object, ChannelUserCompleter::ChannelUserCompleterType);

    [[nodiscard]] bool operator==(const ChannelUserCompleter &other) const;

    [[nodiscard]] QString description() const;
    void setDescription(const QString &newDescription);

    [[nodiscard]] QString name() const;
    void setName(const QString &newName);

    [[nodiscard]] ChannelUserCompleterType type() const;
    void setType(ChannelUserCompleterType newType);

    [[nodiscard]] QIcon statusIcon() const;

    [[nodiscard]] QIcon channelUserIcon() const;

    [[nodiscard]] QString userName() const;
    void setUserName(const QString &newUserName);

private:
    QString mDescription;
    QString mName;
    QString mUserName;
    QIcon mStatusIcon;
    QIcon mChannelUserIcon;

    ChannelUserCompleterType mType = ChannelUserCompleterType::Unknown;
};
Q_DECLARE_TYPEINFO(ChannelUserCompleter, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ChannelUserCompleter &t);
