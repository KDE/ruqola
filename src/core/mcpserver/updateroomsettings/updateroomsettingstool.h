/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "mcpserver/common/toolbase.h"

class UpdateRoomSettingsTool : public ToolBase
{
public:
    explicit UpdateRoomSettingsTool(AccountManager *accountManager);
    ~UpdateRoomSettingsTool() override;

    [[nodiscard]] TextAutoGenerateTextMcpProtocolCore::McpProtocolTool definition() const override;
    void call(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call) override;
};
