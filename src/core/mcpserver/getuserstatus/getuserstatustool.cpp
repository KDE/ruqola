/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "getuserstatustool.h"
#include "mcpserver/common/toolutils.h"
#include "model/usersmodel.h"
#include "rocketchataccount.h"
#include "utils.h"
#include <KLocalizedString>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerToolCall>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolToolAnnotations>

using namespace Qt::Literals::StringLiterals;
GetUserStatusTool::GetUserStatusTool(AccountManager *accountManager)
    : ToolBase(accountManager)
{
}

GetUserStatusTool::~GetUserStatusTool() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolTool GetUserStatusTool::definition() const
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolTool tool = TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::createTool(
        u"get_user_status"_s,
        i18n("Get User Status"),
        u"Get the presence status of a user"_s,
        TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::ToolKind::ReadOnly,
        QMap<QString, QJsonObject>{
            {u"accountName"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Account name (see list_accounts); current account if omitted"_s)},
            {u"userName"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"User name (without @)"_s)}},
        {u"userName"_s});

    ToolUtils::addToolMeta(tool, ToolUtils::CurrentUser);
    return tool;
}

void GetUserStatusTool::doCall(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call)
{
    const RocketChatAccount *account = accountFromArguments(call);
    if (!account) {
        call->finishWithText(i18n("No account found"), true);
        return;
    }
    QString userName = call->arguments().value(u"userName"_s).toString();
    if (userName.startsWith(u'@')) {
        userName.remove(0, 1);
    }
    if (userName.isEmpty()) {
        call->finishWithText(i18n("Missing user name"), true);
        return;
    }

    QStringList entries;
    entries.append(u"user: %1"_s.arg(userName));
    // The users model only contains users whose presence was reported by the server, others are offline
    if (const User user = account->usersModel()->userFromUserName(userName); user.isValid()) {
        if (const QString name = user.name(); !name.isEmpty()) {
            entries.append(u"name: %1"_s.arg(name));
        }
        entries.append(u"status: %1"_s.arg(Utils::presenceStatusToString(user.status())));
        if (const QString statusText = user.statusText(); !statusText.isEmpty()) {
            entries.append(u"status text: %1"_s.arg(statusText));
        }
    } else {
        entries.append(u"status: %1"_s.arg(Utils::presenceStatusToString(User::PresenceStatus::Offline)));
    }

    call->finishWithText(entries.join(u'\n'));
}
