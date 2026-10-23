/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerProtocolManager>
class RocketChatAccount;
class McpServerManager : public TextAutoGenerateTextMcpProtocolCore::McpProtocolServerProtocolManager
{
    Q_OBJECT
public:
    explicit McpServerManager(RocketChatAccount *account, QObject *parent = nullptr);
    ~McpServerManager() override;

private:
    RocketChatAccount *const mAccount;
};
