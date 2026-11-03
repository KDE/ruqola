/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "misc/directoryjob.h"
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerTool>
#include <functional>
class AccountManager;
class RocketChatAccount;

class ToolBase : public TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool
{
public:
    explicit ToolBase(AccountManager *accountManager);
    ~ToolBase();

protected:
    [[nodiscard]] RocketChatAccount *accountFromArguments(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call) const;
    // Search server directory, \a done is called with the reply (call is answered with an error on failure)
    void searchDirectory(RocketChatAccount *account,
                         TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call,
                         RocketChatRestApi::DirectoryJob::SearchType searchType,
                         const std::function<void(const QJsonObject &)> &done) const;

    AccountManager *const mAccountManager;
};
