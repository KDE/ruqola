/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "utils.h"
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
        Notification,
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

    [[nodiscard]] QString userName() const;
    void setUserName(const QString &newUserName);

    [[nodiscard]] QString avatarTag() const;
    void setAvatarTag(const QString &newAvatarTag);

    [[nodiscard]] bool outsideRoom() const;
    void setOutsideRoom(bool newOutsideRoom);

    [[nodiscard]] QString completerName() const;

    [[nodiscard]] QString identifier() const;
    void setIdentifier(const QString &newIdentifier);

    [[nodiscard]] Utils::AvatarInfo avatarInfo() const;

    void setChannelIcon();

    void setAvatarInfo(const Utils::AvatarInfo &newAvatarInfo);

private:
    LIBRUQOLACORE_NO_EXPORT void createAvatarInfo();
    Utils::AvatarInfo mAvatarInfo;
    QIcon mStatusIcon;
    QString mDescription;
    QString mName;
    QString mUserName;
    QString mAvatarTag;
    QString mIdentifier;

    ChannelUserCompleterType mType = ChannelUserCompleterType::Unknown;
    bool mOutsideRoom = false;
};
Q_DECLARE_TYPEINFO(ChannelUserCompleter, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ChannelUserCompleter &t);
