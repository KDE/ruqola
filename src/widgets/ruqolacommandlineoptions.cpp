/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolacommandlineoptions.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>

RuqolaCommandLineParser::RuqolaCommandLineParser(QCommandLineParser *parser)
{
    initializeCommandLine(parser);
}

RuqolaCommandLineParser::~RuqolaCommandLineParser() = default;

QString RuqolaCommandLineParser::commandLineFromEnum(CommandLineName e)
{
    switch (e) {
    case CommandLineName::ListAccount:
        return u"list-accounts"_s;
    case CommandLineName::Account:
        return u"account"_s;
    case CommandLineName::MessageUrl:
        return u"messageurl"_s;
    case CommandLineName::LoginDdpApi:
        return u"loginddpapi"_s;
#if HAVE_KUSERFEEDBACK
    case CommandLineName::FeedBack:
        return u"feedback"_s;
#endif
    }
    return {};
}

void RuqolaCommandLineParser::initializeCommandLine(QCommandLineParser *parser)
{
    parser->addOption(QCommandLineOption(QStringList() << commandLineFromEnum(CommandLineName::ListAccount), i18nc("@info:shell", "Return lists of accounts")));
    parser->addOption(QCommandLineOption(QStringList() << commandLineFromEnum(CommandLineName::Account),
                                         i18nc("@info:shell", "Start with specific account"),
                                         u"accountname"_s));
    parser->addOption(QCommandLineOption(QStringList() << commandLineFromEnum(CommandLineName::MessageUrl), i18nc("@info:shell", "Show Message"), u"url"_s));
    parser->addOption(QCommandLineOption(QStringList() << commandLineFromEnum(CommandLineName::LoginDdpApi), i18nc("@info:shell", "Use ddp api for login")));
#if HAVE_KUSERFEEDBACK
    parser->addOption(
        QCommandLineOption(commandLineFromEnum(CommandLineName::FeedBack), i18nc("@info:shell", "Lists the available options for user feedback")));
#endif
}
