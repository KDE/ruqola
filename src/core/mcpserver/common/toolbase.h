/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "config-ruqola.h"
#include "misc/directoryjob.h"
#if HAVE_MCP_CONFIRMATION_TOOL
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerConfirmationTool>
#else
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerTool>
#endif
#include <functional>
class AccountManager;
class RocketChatAccount;

#if HAVE_MCP_CONFIRMATION_TOOL
class ToolBase : public TextAutoGenerateTextMcpProtocolCore::McpProtocolServerConfirmationTool
#else
class ToolBase : public TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool
#endif
{
public:
    explicit ToolBase(AccountManager *accountManager);
    ~ToolBase() override;

#if !HAVE_MCP_CONFIRMATION_TOOL
    void call(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call) final;
#endif

protected:
#if HAVE_MCP_CONFIRMATION_TOOL
    [[nodiscard]] bool askModify() const override;
#endif
    [[nodiscard]] RocketChatAccount *accountFromArguments(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call) const;
    // Search server directory, \a done is called with the reply (call is answered with an error on failure)
    void searchDirectory(RocketChatAccount *account,
                         TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call,
                         RocketChatRestApi::DirectoryJob::SearchType searchType,
                         const std::function<void(const QJsonObject &)> &done) const;

    AccountManager *const mAccountManager;
#if !HAVE_MCP_CONFIRMATION_TOOL

private:
    virtual void doCall(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call) = 0;
#endif
};
