/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "toolbase.h"
#include "accountmanager.h"
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerToolCall>
using namespace Qt::Literals::StringLiterals;

ToolBase::ToolBase(AccountManager *accountManager)
    : TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool()
    , mAccountManager(accountManager)
{
}

ToolBase::~ToolBase() = default;

RocketChatAccount *ToolBase::accountFromArguments(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call) const
{
    const QJsonValue accountNameValue = call->arguments().value(u"accountName"_s);
    if (!accountNameValue.isString()) {
        return mAccountManager->account();
    }
    return mAccountManager->accountFromName(accountNameValue.toString());
}
