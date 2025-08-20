/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "config-ruqola.h"
#include "libruqolawidgets_export.h"
#include <QCommandLineParser>
#include <QString>

class LIBRUQOLAWIDGETS_EXPORT RuqolaCommandLineParser
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
    };

    explicit RuqolaCommandLineParser(QCommandLineParser *parser);
    ~RuqolaCommandLineParser();

    [[nodiscard]] static QString commandLineFromEnum(CommandLineName e);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void initializeCommandLine(QCommandLineParser *parser);
};
