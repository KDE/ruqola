/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolacommandlineparser.h"

#include <KLocalizedString>

using namespace Qt::Literals::StringLiterals;
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
#if ADD_OFFLINE_SUPPORT
    case CommandLineName::Offline:
        return u"offline"_s;
#endif
    case CommandLineName::Debug:
        return u"debug"_s;
    }
    return {};
}

void RuqolaCommandLineParser::initializeCommandLine(QCommandLineParser *parser)
{
    parser->addOption(QCommandLineOption(QStringList() << commandLineFromEnum(CommandLineName::ListAccount), i18nc("@info:shell", "Return lists of accounts")));
    parser->addOption(QCommandLineOption(QStringList() << commandLineFromEnum(CommandLineName::Account),
                                         i18nc("@info:shell", "Start with specific account"),
                                         i18n("Account Name")));
    parser->addOption(
        QCommandLineOption(QStringList() << commandLineFromEnum(CommandLineName::MessageUrl), i18nc("@info:shell", "Show Message"), i18n("Message Url")));
    parser->addOption(QCommandLineOption(QStringList() << commandLineFromEnum(CommandLineName::LoginDdpApi), i18nc("@info:shell", "Use ddp api for login")));
#if HAVE_KUSERFEEDBACK
    parser->addOption(
        QCommandLineOption(commandLineFromEnum(CommandLineName::FeedBack), i18nc("@info:shell", "Lists the available options for user feedback")));
#endif
#if ADD_OFFLINE_SUPPORT
    parser->addOption(QCommandLineOption(QStringList() << commandLineFromEnum(CommandLineName::Offline), i18nc("@info:shell", "Start as Offline")));
#endif
    parser->addOption(QCommandLineOption(QStringList() << commandLineFromEnum(CommandLineName::Debug), i18nc("@info:shell", "Activate Debug Mode")));
}
