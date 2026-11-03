/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "getmystatustool.h"
#include "model/statusmodel.h"
#include "rocketchataccount.h"
#include "utils.h"
#include <KLocalizedString>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerToolCall>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>

using namespace Qt::Literals::StringLiterals;
GetMyStatusTool::GetMyStatusTool(AccountManager *accountManager)
    : ToolBase(accountManager)
{
}

GetMyStatusTool::~GetMyStatusTool() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolTool GetMyStatusTool::definition() const
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolTool tool = TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::createTool(
        u"get_my_status"_s,
        i18n("Get My Status"),
        u"Get the current presence status of the logged-in user of an account"_s,
        TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::ToolKind::ReadOnly,
        QMap<QString, QJsonObject>{
            {u"accountName"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Account name (see list_accounts); current account if omitted"_s)}});
    return tool;
}

void GetMyStatusTool::call(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call)
{
    const RocketChatAccount *account = accountFromArguments(call);
    if (!account) {
        call->finishWithText(i18n("No account found"), true);
        return;
    }

    QStringList entries;
    entries.append(u"account: %1"_s.arg(account->accountName()));
    entries.append(u"user: %1"_s.arg(account->userName()));
    entries.append(u"status: %1"_s.arg(Utils::presenceStatusToString(account->presenceStatus())));
    if (const QString statusText = account->statusModel()->customText(); !statusText.isEmpty()) {
        entries.append(u"status text: %1"_s.arg(statusText));
    }

    call->finishWithText(entries.join(u'\n'));
}
