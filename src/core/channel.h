/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
// TODO rename it
class LIBRUQOLACORE_EXPORT Channel
{
    Q_GADGET
public:
    enum ChannelType {
        Room,
        DirectChannel,
        Unknown,
    };
    Q_ENUM(ChannelType)

    Channel();
    ~Channel();
    void parseChannel(const QJsonObject &object, Channel::ChannelType);

    [[nodiscard]] ChannelType type() const;
    void setType(Channel::ChannelType type);

    [[nodiscard]] bool operator==(const Channel &other) const;

    Channel &operator=(const Channel &other) = default;

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &roomId);

    [[nodiscard]] QString roomName() const;
    void setRoomName(const QString &roomName);

    [[nodiscard]] QString roomType() const;
    void setRoomType(const QString &roomType);

    [[nodiscard]] QString userId() const;
    void setUserId(const QString &userId);

    [[nodiscard]] QString name() const;
    void setName(const QString &name);

    [[nodiscard]] QString status() const;
    void setStatus(const QString &status);

    [[nodiscard]] QString userName() const;
    void setUserName(const QString &userName);

    [[nodiscard]] QString iconFromStatus() const;

private:
    QString mRoomId;
    QString mRoomName;
    QString mRoomType;

    // Private Channel
    QString mUserId;
    QString mName;
    QString mStatus;
    QString mUserName;
    ChannelType mType = ChannelType::Unknown;
};
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
Q_DECLARE_METATYPE(Channel)
#endif
Q_DECLARE_TYPEINFO(Channel, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Channel &t);
