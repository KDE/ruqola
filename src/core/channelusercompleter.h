/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "utils.h"
#include <QIcon>

class LIBRUQOLACORE_EXPORT ChannelUserCompleter
{
    Q_GADGET
public:
    ChannelUserCompleter();
    ~ChannelUserCompleter();

    enum ChannelUserCompleterType : uint8_t {
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

    [[nodiscard]] QByteArray identifier() const;
    void setIdentifier(const QByteArray &newIdentifier);

    [[nodiscard]] Utils::AvatarInfo avatarInfo() const;

    void setChannelIcon();

    void setAvatarInfo(const Utils::AvatarInfo &newAvatarInfo);

    [[nodiscard]] QString fName() const;
    void setFName(const QString &newFName);

private:
    LIBRUQOLACORE_NO_EXPORT void createAvatarInfo();
    Utils::AvatarInfo mAvatarInfo;
    QIcon mStatusIcon;
    QString mDescription;
    QString mName;
    QString mFName;
    QString mUserName;
    QString mAvatarTag;
    QByteArray mIdentifier;

    ChannelUserCompleterType mType = ChannelUserCompleterType::Unknown;
    bool mOutsideRoom = false;
};
Q_DECLARE_TYPEINFO(ChannelUserCompleter, Q_RELOCATABLE_TYPE);
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ChannelUserCompleter &t);
