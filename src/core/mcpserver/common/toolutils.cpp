/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "toolutils.h"

#include <KLocalizedString>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolCommonUtils>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>
using namespace Qt::Literals::StringLiterals;

QString ToolUtils::groupTitleFromType(TitleType type)
{
    switch (type) {
    case Rooms:
        return i18n("Rooms");
    case Messages:
        return i18n("Messages");
    case Users:
        return i18n("Users");
    case CurrentUser:
        return i18n("Current User");
    case Misc:
        return i18n("Misc");
    }
    Q_UNREACHABLE();
    return {};
}

QString ToolUtils::groupFromType(TitleType type)
{
    switch (type) {
    case Rooms:
        return u"rooms"_s;
    case Messages:
        return u"messages"_s;
    case Users:
        return u"users"_s;
    case CurrentUser:
        return u"currentuser"_s;
    case Misc:
        return u"misc"_s;
    }
    Q_UNREACHABLE();
    return {};
}

void ToolUtils::addToolMeta(TextAutoGenerateTextMcpProtocolCore::McpProtocolMeta &meta, TitleType type)
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolCommonUtils::addToolMeta(meta, groupFromType(type), groupTitleFromType(type));
}

void ToolUtils::addToolMeta(TextAutoGenerateTextMcpProtocolCore::McpProtocolTool &tool, TitleType type)
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolMeta meta = tool.meta().value_or(TextAutoGenerateTextMcpProtocolCore::McpProtocolMeta{});
    addToolMeta(meta, type);
    tool.setMeta(meta);
}
