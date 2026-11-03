/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "searchuserstool.h"
#include "users.h"
#include <KLocalizedString>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerToolCall>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>

using namespace Qt::Literals::StringLiterals;
SearchUsersTool::SearchUsersTool(AccountManager *accountManager)
    : ToolBase(accountManager)
{
}

SearchUsersTool::~SearchUsersTool() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolTool SearchUsersTool::definition() const
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolTool tool = TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::createTool(
        u"search_users"_s,
        i18n("Search Users"),
        u"Search the users of the server directory by user name or name"_s,
        TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::ToolKind::ReadOnly,
        QMap<QString, QJsonObject>{
            {u"accountName"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Account name (see list_accounts); current account if omitted"_s)},
            {u"pattern"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Text to search in user names and names (without @)"_s)},
            {u"count"_s, McpProtocolServerTool::schemaProperty(u"integer"_s, u"Maximum number of results (1-100); 25 if omitted"_s)}},
        {u"pattern"_s});
    return tool;
}

void SearchUsersTool::call(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call)
{
    RocketChatAccount *account = accountFromArguments(call);
    if (!account) {
        call->finishWithText(i18n("No account found"), true);
        return;
    }
    searchDirectory(account, call, RocketChatRestApi::DirectoryJob::SearchType::Users, [call](const QJsonObject &obj) {
        Users users;
        users.parseUsers(obj, Users::ParseType::Directory, {});
        if (users.isEmpty()) {
            call->finishWithText(i18n("No users found"));
            return;
        }
        QStringList entries;
        entries.reserve(users.count() + 1);
        entries.append(u"found: %1 (total: %2)"_s.arg(users.count()).arg(users.total()));
        for (const User &user : users.list()) {
            QString entry = user.userName();
            if (const QString name = user.name(); !name.isEmpty()) {
                entry += u" (name: %1, id: %2)"_s.arg(name, QString::fromLatin1(user.userId()));
            } else {
                entry += u" (id: %1)"_s.arg(QString::fromLatin1(user.userId()));
            }
            entries.append(entry);
        }
        call->finishWithText(entries.join(u'\n'));
    });
}
