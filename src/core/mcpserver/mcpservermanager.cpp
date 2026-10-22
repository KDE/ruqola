/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "mcpservermanager.h"
#include <KLocalizedString>
namespace
{
constexpr int toolsPageSize = 2;
}
using namespace Qt::Literals::StringLiterals;
McpServerManager::McpServerManager(QObject *parent)
    : TextAutoGenerateTextMcpProtocolCore::McpProtocolServerProtocolManager{
          TextAutoGenerateTextMcpProtocolCore::McpProtocolPlugin::TransportType::StreamableHttp,
          parent}
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolImplementation serverInfo;
    serverInfo.setName(u"ruqola_mcp_protocol"_s);
    serverInfo.setVersion(u"1.0"_s);
    setServerInfo(serverInfo);
    setInstructions(i18n("Manage information from Ruqola"));
    setToolsPageSize(toolsPageSize);
}

McpServerManager::~McpServerManager() = default;
#include "moc_mcpservermanager.cpp"
