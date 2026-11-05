/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QString>

namespace TextAutoGenerateTextMcpProtocolCore
{
class McpProtocolMeta;
class McpProtocolTool;
}
namespace ToolUtils
{
enum TitleType : uint8_t {
    Rooms,
    Messages,
    Users,
    CurrentUser,
    Misc,
};

void addToolMeta(TextAutoGenerateTextMcpProtocolCore::McpProtocolTool &tool, TitleType type);
[[nodiscard]] QString groupFromType(TitleType type);
[[nodiscard]] QString groupTitleFromType(TitleType type);
}
