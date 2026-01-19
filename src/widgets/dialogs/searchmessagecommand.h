/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QMetaType>

class LIBRUQOLAWIDGETS_TESTS_EXPORT SearchMessageCommand
{
    Q_GADGET
public:
    SearchMessageCommand();
    ~SearchMessageCommand();

    enum SearchMessageCommandType : uint8_t {
        Unknown = 0,
        HasStar,
        IsPinned,
        HasUrl,
        HasLocation,
        Before,
        After,
        Day,
        Order,
        FromMe,
        FromUserName,
    };
    Q_ENUM(SearchMessageCommandType)

    [[nodiscard]] static QString generateCommandText(SearchMessageCommand::SearchMessageCommandType type);
    [[nodiscard]] static bool mustBeUnique(SearchMessageCommand::SearchMessageCommandType type);
};
