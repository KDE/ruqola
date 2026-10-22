/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "listaccountstool.h"
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>
using namespace Qt::Literals::StringLiterals;
ListAccountsTool::ListAccountsTool() = default;
ListAccountsTool::~ListAccountsTool() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolTool ListAccountsTool::definition() const
{
    // TODO
    return {};
}

void ListAccountsTool::call(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call)
{
    // TODO
}
