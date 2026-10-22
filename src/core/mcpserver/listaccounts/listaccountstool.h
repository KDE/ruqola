/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerTool>

class ListAccountsTool : public TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool
{
public:
    ListAccountsTool();
    ~ListAccountsTool() override;

    [[nodiscard]] TextAutoGenerateTextMcpProtocolCore::McpProtocolTool definition() const override;
    void call(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call) override;
};
