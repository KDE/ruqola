/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolacore_export.h"
#include <QDate>
#include <QMetaType>
#include <QObject>
#include <QTime>
class QDebug;
class LIBRUQOLACORE_EXPORT TimeStampInMessagesUtils
{
    Q_GADGET
public:
    enum class FormatType : uint8_t {
        Unknown = 0,
        ShortTime,
        LongTime,
        ShortDate,
        LongDate,
        FullDateTime,
        LongFullDateTime,
        RelativeTime,
    };
    Q_ENUM(FormatType)

    [[nodiscard]] static QString convertFormatTypeToString(TimeStampInMessagesUtils::FormatType type);

    struct LIBRUQOLACORE_EXPORT TimeStampInfo {
        TimeStampInMessagesUtils::FormatType format = FormatType::Unknown;
        QDate date;
        QTime time;
        QString timeZone;
        [[nodiscard]] bool isValid() const;
    };
    [[nodiscard]] static QString generateTimeStampStr(const TimeStampInfo &info);
};
Q_DECLARE_TYPEINFO(TimeStampInMessagesUtils::TimeStampInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const TimeStampInMessagesUtils::TimeStampInfo &t);
