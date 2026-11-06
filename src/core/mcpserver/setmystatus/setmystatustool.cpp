/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "setmystatustool.h"
#include "mcpserver/common/toolutils.h"
#include "rocketchataccount.h"
#include "utils.h"
#include <KLocalizedString>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerToolCall>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>

using namespace Qt::Literals::StringLiterals;
SetMyStatusTool::SetMyStatusTool(AccountManager *accountManager)
    : ToolBase(accountManager)
{
}

SetMyStatusTool::~SetMyStatusTool() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolTool SetMyStatusTool::definition() const
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolTool tool = TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::createTool(
        u"set_my_status"_s,
        i18n("Set My Status"),
        u"Set the presence status of the logged-in user of an account, with an optional status text"_s,
        TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::ToolKind::Modify,
        QMap<QString, QJsonObject>{
            {u"accountName"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Account name (see list_accounts); current account if omitted"_s)},
            {u"status"_s, McpProtocolServerTool::schemaEnumProperty({u"online"_s, u"busy"_s, u"away"_s, u"offline"_s}, u"New presence status"_s)},
            {u"statusText"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Custom status text; cleared if omitted"_s)}},
        {u"status"_s});
    return tool;
}

void SetMyStatusTool::doCall(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call)
{
    RocketChatAccount *account = accountFromArguments(call);
    if (!account) {
        call->finishWithText(i18n("No account found"), true);
        return;
    }
    if (account->loginStatus() != AuthenticationManager::LoggedIn) {
        call->finishWithText(i18n("Account is not logged in"), true);
        return;
    }
    const User::PresenceStatus status = Utils::presenceStatusFromString(call->arguments().value(u"status"_s).toString());
    if (status == User::PresenceStatus::Unknown) {
        call->finishWithText(i18n("Invalid status"), true);
        return;
    }
    const QString statusText = call->arguments().value(u"statusText"_s).toString();
    account->setDefaultStatus(status, statusText);

    QStringList entries;
    entries.append(u"account: %1"_s.arg(account->accountName()));
    entries.append(u"status: %1"_s.arg(Utils::presenceStatusToString(status)));
    if (!statusText.isEmpty()) {
        entries.append(u"status text: %1"_s.arg(statusText));
    }
    call->finishWithText(entries.join(u'\n'));
}
