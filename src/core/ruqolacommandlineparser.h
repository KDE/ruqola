/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "config-ruqola.h"
#include "libruqolacore_export.h"
#include <QCommandLineParser>
#include <QString>

class LIBRUQOLACORE_EXPORT RuqolaCommandLineParser
{
public:
    enum class CommandLineName : uint8_t {
        ListAccount,
        Account,
        MessageUrl,
        LoginDdpApi,
#if HAVE_KUSERFEEDBACK
        FeedBack,
#endif
#if ADD_OFFLINE_SUPPORT
        Offline,
#endif
        CleanDatabase,
        Debug,
    };

    explicit RuqolaCommandLineParser(QCommandLineParser *parser);
    ~RuqolaCommandLineParser();

    [[nodiscard]] static QString commandLineFromEnum(CommandLineName e);

private:
    LIBRUQOLACORE_NO_EXPORT void initializeCommandLine(QCommandLineParser *parser);
};
