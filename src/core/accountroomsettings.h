/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QMap>
#include <QString>

class LIBRUQOLACORE_EXPORT AccountRoomSettings
{
public:
    struct LIBRUQOLACORE_EXPORT PendingTypedInfo {
        QString text;
        QString messageIdBeingEdited;
        QString threadMessageId;
        QString quotePermalink;
        QString quoteText;
        int scrollbarPosition = -1;
        [[nodiscard]] bool isValid() const;
    };
    AccountRoomSettings();
    ~AccountRoomSettings();

    void remove(const QString &roomId);

    void add(const QString &roomId, const PendingTypedInfo &info);

    [[nodiscard]] PendingTypedInfo value(const QString &roomId);

    [[nodiscard]] bool isEmpty() const;

private:
    QMap<QString /*RoomId*/, PendingTypedInfo> mPendingTypedTexts;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AccountRoomSettings::PendingTypedInfo &t);
