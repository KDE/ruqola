/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolacommandlineoptions.h"
#include "config-ruqola.h"
#include <KLocalizedString>

RuqolaCommandLineParser::RuqolaCommandLineParser(QCommandLineParser *parser)
{
    initializeCommandLine(parser);
}

RuqolaCommandLineParser::~RuqolaCommandLineParser() = default;

QString RuqolaCommandLineParser::commandLineFromEnum(CommandLineName e)
{
    switch (e) {
    case ListAccount:
        return QStringLiteral("list-accounts");
    case Account:
        return QStringLiteral("account");
    case MessageUrl:
        return QStringLiteral("messageurl");
    case LoginDdpApi:
        return QStringLiteral("loginddpapi");
    case FeedBack:
        return QStringLiteral("feedback");
    }
    return {};
}

void RuqolaCommandLineParser::initializeCommandLine(QCommandLineParser *parser)
{
    parser->addOption(QCommandLineOption(QStringList() << commandLineFromEnum(ListAccount), i18nc("@info:shell", "Return lists of accounts")));
    parser->addOption(
        QCommandLineOption(QStringList() << commandLineFromEnum(Account), i18nc("@info:shell", "Start with specific account"), QStringLiteral("accountname")));
    parser->addOption(QCommandLineOption(QStringList() << commandLineFromEnum(MessageUrl), i18nc("@info:shell", "Show Message"), QStringLiteral("url")));
    parser->addOption(QCommandLineOption(QStringList() << commandLineFromEnum(LoginDdpApi), i18nc("@info:shell", "Use ddp api for login")));

#if HAVE_KUSERFEEDBACK
    parser->addOption(QCommandLineOption(commandLineFromEnum(FeedBack), i18nc("@info:shell", "Lists the available options for user feedback")));
#endif
}
