/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "toolbase.h"

ToolBase::ToolBase(AccountManager *accountManager)
    : TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool()
    , mAccountManager(accountManager)
{
}

ToolBase::~ToolBase() = default;
