
/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqola_private_export.h"
#include "timestampinmessagesutils.h"
#include <QString>
class QDateTime;
class LIBRUQOLACORE_TESTS_EXPORT TimeStampInMessagesConverter
{
public:
    TimeStampInMessagesConverter();
    ~TimeStampInMessagesConverter();

    [[nodiscard]] QString generateTimeStamp(const QString &str) const;
    [[nodiscard]] static QString convertTimeStamp(const QDateTime &dateTime, TimeStampInMessagesUtils::FormatType type);
    [[nodiscard]] static QString calculateRelativeTime(const QDateTime &dateTime, const QDateTime &currentDateTime);
    [[nodiscard]] QString regularExpressionStr() const;
};
