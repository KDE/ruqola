/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "mcpservermanager.h"
#include "accountmanager.h"
#include "listaccounts/listaccountstool.h"
#include "mcpserver/getroominfo/getroominfotool.h"
#include "mcpserver/listrooms/listroomstool.h"
#include "ruqola_mcp_server_debug.h"
#include "ruqolaglobalconfig.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolSettings>
namespace
{
constexpr int toolsPageSize = 10;
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
    addTool(std::make_unique<ListRoomsTool>(mAccountManager));
    addTool(std::make_unique<GetRoomInfoTool>(mAccountManager));
    loadConfig();
    // TODO add more
}

McpServerManager::~McpServerManager() = default;

void McpServerManager::reloadConfig()
{
    if (isRunning()) {
        stop();
    }
    loadConfig();
}

void McpServerManager::loadConfig()
{
    if (RuqolaGlobalConfig::self()->enableMcpServer()) {
        TextAutoGenerateTextMcpProtocolCore::McpProtocolSettings settings;
        const KConfigGroup group(KSharedConfig::openConfig(), u"mcpserver"_s);
        settings.load(group);
        if (!settings.isValid()) {
            qCWarning(RUQOLA_MCP_SERVER_LOG) << "MCP Server settings is invalid";
            return; // TODO return or opening settings ?
        }
        setSettings(settings);
        start();
    }
}
#include "moc_mcpservermanager.cpp"
