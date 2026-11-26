/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolacommandlineparser.h"

#include <KLocalizedString>
#if HAVE_MCP_TOOL_LIST
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerUtils>
#else
#include <QTextStream>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>
#include <algorithm>
#endif

using namespace Qt::Literals::StringLiterals;
RuqolaCommandLineParser::RuqolaCommandLineParser(QCommandLineParser *parser)
{
    initializeCommandLine(parser);
}

QString RuqolaCommandLineParser::commandLineFromEnum(CommandLineName e)
{
    switch (e) {
    case CommandLineName::ListAccount:
        return u"list-accounts"_s;
    case CommandLineName::Account:
        return u"account"_s;
    case CommandLineName::MessageUrl:
        return u"messageurl"_s;
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
    case CommandLineName::CleanDatabase:
        return u"clean-database"_s;
#if HAVE_TEXT_AUTOGENERATE_MCP_PROTOCOL
    case CommandLineName::ListTools:
        return u"list-tools"_s;
#endif
    }
    return {};
}

void RuqolaCommandLineParser::initializeCommandLine(QCommandLineParser *parser)
{
    parser->addOption(QCommandLineOption(commandLineFromEnum(CommandLineName::ListAccount), i18nc("@info:shell", "Return lists of accounts")));
    parser->addOption(
        QCommandLineOption(commandLineFromEnum(CommandLineName::Account), i18nc("@info:shell", "Start with specific account"), i18n("Account Name")));
    parser->addOption(QCommandLineOption(commandLineFromEnum(CommandLineName::MessageUrl), i18nc("@info:shell", "Show Message"), i18n("Message Url")));
#if HAVE_KUSERFEEDBACK
    parser->addOption(
        QCommandLineOption(commandLineFromEnum(CommandLineName::FeedBack), i18nc("@info:shell", "Lists the available options for user feedback")));
#endif
#if ADD_OFFLINE_SUPPORT
    parser->addOption(QCommandLineOption(commandLineFromEnum(CommandLineName::Offline), i18nc("@info:shell", "Start as Offline")));
#endif
    parser->addOption(QCommandLineOption(commandLineFromEnum(CommandLineName::Debug), i18nc("@info:shell", "Activate Debug Mode")));
    parser->addOption(QCommandLineOption(commandLineFromEnum(CommandLineName::CleanDatabase), i18nc("@info:shell", "Clean Database (Use with caution)")));
#if HAVE_TEXT_AUTOGENERATE_MCP_PROTOCOL
    parser->addOption(
        QCommandLineOption(QStringList() << commandLineFromEnum(CommandLineName::ListTools), i18nc("@info:shell", "List supported MCP tools and exit.")));
#endif
}

#if HAVE_TEXT_AUTOGENERATE_MCP_PROTOCOL
void RuqolaCommandLineParser::printTools(const QList<TextAutoGenerateTextMcpProtocolCore::McpProtocolTool> &lst)
{
#if HAVE_MCP_TOOL_LIST
    TextAutoGenerateTextMcpProtocolCore::McpProtocolServerUtils::printTools(lst);
#else
    qsizetype nameWidth = 0;
    for (const auto &tool : lst) {
        nameWidth = std::max(nameWidth, tool.name().size());
    }
    QTextStream out(stdout);
    out << i18n("List of Tools:") << '\n';
    for (const auto &tool : lst) {
        out << "   " << tool.name().leftJustified(nameWidth) << "   " << tool.description().value_or(tool.title().value_or(QString())) << '\n';
    }
#endif
}
#endif
