/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "mcpserver/common/toolbase.h"

class GetRoomRolesTool : public ToolBase
{
public:
    explicit GetRoomRolesTool(AccountManager *accountManager);
    ~GetRoomRolesTool() override;

    [[nodiscard]] TextAutoGenerateTextMcpProtocolCore::McpProtocolTool definition() const override;
    void call(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call) override;
};
