/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "mcpserver/common/toolbase.h"

class GetMessageTool : public ToolBase
{
public:
    explicit GetMessageTool(AccountManager *accountManager);
    ~GetMessageTool() override;

    [[nodiscard]] TextAutoGenerateTextMcpProtocolCore::McpProtocolTool definition() const override;

private:
    void doCall(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call) override;
};
