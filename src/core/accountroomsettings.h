/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>
#include <QMap>
#include <QString>
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT AccountRoomSettings
{
public:
    struct LIBRUQOLACORE_EXPORT PendingTypedInfo {
        QString text;
        QByteArray messageIdBeingEdited;
        QByteArray threadMessageId;
        QString quotePermalink;
        QString quoteText;
        int scrollbarPosition = -1;
        [[nodiscard]] bool isValid() const;
        [[nodiscard]] bool hasPendingMessageTyped() const;
        [[nodiscard]] static QJsonObject serialize(const PendingTypedInfo &url);
        [[nodiscard]] static PendingTypedInfo deserialize(const QJsonObject &o);
        [[nodiscard]] bool operator==(const PendingTypedInfo &other) const;
    };
    explicit AccountRoomSettings(RocketChatAccount *account);
    ~AccountRoomSettings();

    void remove(const QByteArray &roomId);

    void add(const QByteArray &roomId, const PendingTypedInfo &info);

    [[nodiscard]] PendingTypedInfo value(const QByteArray &roomId);
    [[nodiscard]] bool hasPendingMessageTyped(const QByteArray &roomId) const;

    [[nodiscard]] bool isEmpty() const;

    void loadRoomPendingTypedInfo();

    void setPendingTypedTexts(const QMap<QByteArray, PendingTypedInfo> &newPendingTypedTexts);

private:
    QMap<QByteArray /*RoomId*/, PendingTypedInfo> mPendingTypedTexts;
    RocketChatAccount *const mRocketChatAccount;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AccountRoomSettings::PendingTypedInfo &t);
