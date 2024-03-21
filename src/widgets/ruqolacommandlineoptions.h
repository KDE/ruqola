/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_export.h"
#include <QCommandLineParser>
#include <QString>

class LIBRUQOLAWIDGETS_EXPORT RuqolaCommandLineParser
{
public:
    enum CommandLineName {
        ListAccount,
        Account,
        MessageUrl,
        LoginRestApi,
        FeedBack,
    };

    explicit RuqolaCommandLineParser(QCommandLineParser *parser);
    ~RuqolaCommandLineParser();

    [[nodiscard]] static QString commandLineFromEnum(CommandLineName e);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void initializeCommandLine(QCommandLineParser *parser);
};
