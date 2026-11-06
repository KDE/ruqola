/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "mcpserver/common/toolbase.h"

class StarMessageTool : public ToolBase
{
public:
    explicit StarMessageTool(AccountManager *accountManager);
    ~StarMessageTool() override;

    [[nodiscard]] TextAutoGenerateTextMcpProtocolCore::McpProtocolTool definition() const override;

private:
    void doCall(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call) override;
};
