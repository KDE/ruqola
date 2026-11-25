/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "config-ruqola.h"
#include "libruqolacore_export.h"
#include <QCommandLineParser>
#include <QString>
#if HAVE_TEXT_AUTOGENERATE_MCP_PROTOCOL
#include <QList>
namespace TextAutoGenerateTextMcpProtocolCore
{
class McpProtocolTool;
}
#endif

class LIBRUQOLACORE_EXPORT RuqolaCommandLineParser
{
public:
    enum class CommandLineName : uint8_t {
        ListAccount,
        Account,
        MessageUrl,
#if HAVE_KUSERFEEDBACK
        FeedBack,
#endif
#if ADD_OFFLINE_SUPPORT
        Offline,
#endif
        CleanDatabase,
        Debug,
#if HAVE_TEXT_AUTOGENERATE_MCP_PROTOCOL
        ListTools,
#endif
    };

    explicit RuqolaCommandLineParser(QCommandLineParser *parser);

    [[nodiscard]] static QString commandLineFromEnum(CommandLineName e);
#if HAVE_TEXT_AUTOGENERATE_MCP_PROTOCOL
    static void printTools(const QList<TextAutoGenerateTextMcpProtocolCore::McpProtocolTool> &lst);
#endif

private:
    static LIBRUQOLACORE_NO_EXPORT void initializeCommandLine(QCommandLineParser *parser);
};
