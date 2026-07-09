/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QMap>
#include <QString>
#include <QUrl>
class QJsonObject;
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT AccountRoomSettings
{
public:
    struct LIBRUQOLACORE_EXPORT PendingAttachmentInfo {
        QUrl fileUrl;
        QString fileName;
        QString name;
        QString alternativeText;
        [[nodiscard]] bool isValid() const
        {
            return fileUrl.isValid() && fileUrl.isLocalFile();
        }
        [[nodiscard]] static QJsonObject serialize(const PendingAttachmentInfo &url);
        [[nodiscard]] static PendingAttachmentInfo deserialize(const QJsonObject &o);
        [[nodiscard]] bool operator==(const PendingAttachmentInfo &other) const;
    };

    struct LIBRUQOLACORE_EXPORT PendingTypedInfo {
        QString text;
        QByteArray messageIdBeingEdited;
        QByteArray threadMessageId;
        QString quotePermalink;
        QString quoteText;
        int scrollbarPosition = -1;
        QList<AccountRoomSettings::PendingAttachmentInfo> pendingAttachmentInfos;
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
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AccountRoomSettings::PendingTypedInfo &t);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AccountRoomSettings::PendingAttachmentInfo &t);
Q_DECLARE_TYPEINFO(AccountRoomSettings::PendingAttachmentInfo, Q_RELOCATABLE_TYPE);
