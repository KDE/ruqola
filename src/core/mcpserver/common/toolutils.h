/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QString>

class QJsonObject;

namespace TextAutoGenerateTextMcpProtocolCore
{
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
// One-line summary of a message (id, date, author, flags) followed by its text
[[nodiscard]] QString messageEntry(const QJsonObject &message, const QString &userId);
}
