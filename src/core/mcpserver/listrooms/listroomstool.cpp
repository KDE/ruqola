/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "listroomstool.h"
#include <KLocalizedString>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerToolCall>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolToolAnnotations>

using namespace Qt::Literals::StringLiterals;
ListRoomsTool::ListRoomsTool(AccountManager *accountManager)
    : ToolBase(accountManager)
{
}

ListRoomsTool::~ListRoomsTool() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolTool ListRoomsTool::definition() const
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolTool tool =
        TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::createTool(u"list_rooms"_s, u"List the accounts"_s);

    TextAutoGenerateTextMcpProtocolCore::McpProtocolToolAnnotations annotation;
    annotation.setReadOnlyHint(true);
    annotation.setOpenWorldHint(false);
    tool.setTitle(i18n("List Rooms"));
    tool.setAnnotations(annotation);
    // TODO ToolUtils::addToolMeta(tool, ToolUtils::Mail);
    return tool;
}

void ListRoomsTool::call(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call)
{
    // TODO
}
