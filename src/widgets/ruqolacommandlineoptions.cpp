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
    case LoginRestApi:
        return QStringLiteral("loginrestapi");
    case FeedBack:
        return QStringLiteral("feedback");
    }
    return {};
}

void RuqolaCommandLineParser::initializeCommandLine(QCommandLineParser *parser)
{
    parser->addOption(QCommandLineOption(QStringList() << commandLineFromEnum(ListAccount), i18n("Return lists of accounts")));
    parser->addOption(QCommandLineOption(QStringList() << commandLineFromEnum(Account), i18n("Start with specific account"), QStringLiteral("accountname")));
    parser->addOption(QCommandLineOption(QStringList() << commandLineFromEnum(MessageUrl), i18n("Show Message"), QStringLiteral("url")));
#if USE_RESTAPI_LOGIN_CMAKE_SUPPORT
    // Laurent don't translate it's temporary otherwise I will change name
    parser->addOption(QCommandLineOption(QStringList() << commandLineFromEnum(LoginRestApi), QStringLiteral("Use restapi for login")));
#endif

#if HAVE_KUSERFEEDBACK
    parser->addOption(QCommandLineOption(commandLineFromEnum(FeedBack), i18n("Lists the available options for user feedback")));
#endif
}
