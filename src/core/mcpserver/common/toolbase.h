/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerTool>
class AccountManager;

class ToolBase : public TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool
{
public:
    explicit ToolBase(AccountManager *accountManager);
    ~ToolBase();

protected:
    AccountManager *const mAccountManager;
};
