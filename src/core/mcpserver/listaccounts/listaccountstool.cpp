/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "listaccountstool.h"
#include <KLocalizedString>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>
using namespace Qt::Literals::StringLiterals;
ListAccountsTool::ListAccountsTool(AccountManager *accountManager)
    : ToolBase(accountManager)
{
}

ListAccountsTool::~ListAccountsTool() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolTool ListAccountsTool::definition() const
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolTool tool =
        TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::createTool(u"list_accounts"_s, u"List the accounts"_s);

    TextAutoGenerateTextMcpProtocolCore::McpProtocolToolAnnotations annotation;
    annotation.setReadOnlyHint(true);
    annotation.setOpenWorldHint(false);
    tool.setTitle(i18n("List Accounts"));
    tool.setAnnotations(annotation);
    // TODO ToolUtils::addToolMeta(tool, ToolUtils::Mail);
    return tool;
}

void ListAccountsTool::call(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call)
{
    // TODO
}
