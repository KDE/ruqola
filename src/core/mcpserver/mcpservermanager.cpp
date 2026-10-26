/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "mcpservermanager.h"
#include "accountmanager.h"
#include "listaccounts/listaccountstool.h"
#include "ruqola_mcp_server_debug.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolSettings>
namespace
{
constexpr int toolsPageSize = 2;
}
using namespace Qt::Literals::StringLiterals;
McpServerManager::McpServerManager(AccountManager *accountManager, QObject *parent)
    : TextAutoGenerateTextMcpProtocolCore::McpProtocolServerProtocolManager{
          TextAutoGenerateTextMcpProtocolCore::McpProtocolPlugin::TransportType::StreamableHttp,
          parent}
    , mAccountManager(accountManager)
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolImplementation serverInfo;
    serverInfo.setName(u"ruqola_mcp_protocol"_s);
    serverInfo.setVersion(u"1.0"_s);
    setServerInfo(serverInfo);
    setInstructions(i18n("Manage information from Ruqola"));
    setToolsPageSize(toolsPageSize);

    addTool(std::make_unique<ListAccountsTool>(mAccountManager));
    loadConfig();
    // TODO add more
}

McpServerManager::~McpServerManager() = default;

void McpServerManager::loadConfig()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolSettings settings;
    const KConfigGroup group(KSharedConfig::openConfig(), u"mcpserver"_s);
    settings.load(group);
    if (!settings.isValid()) {
        qCWarning(RUQOLA_MCP_SERVER_LOG) << "MCP Server settings is invalid";
        return; // TODO return or opening settings ?
    }
    setSettings(settings);
}
#include "moc_mcpservermanager.cpp"
