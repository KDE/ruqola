/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerProtocolManager>
class AccountManager;
class McpServerManager : public TextAutoGenerateTextMcpProtocolCore::McpProtocolServerProtocolManager
{
    Q_OBJECT
public:
    explicit McpServerManager(AccountManager *accountManager, QObject *parent = nullptr);
    ~McpServerManager() override;

    void reloadConfig();

private:
    void loadConfig();
    AccountManager *const mAccountManager;
};
