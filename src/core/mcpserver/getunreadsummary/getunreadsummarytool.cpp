/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "getunreadsummarytool.h"
#include "mcpserver/common/toolutils.h"
#include "model/roommodel.h"
#include "rocketchataccount.h"
#include "room.h"
#include <KLocalizedString>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerToolCall>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolToolAnnotations>

using namespace Qt::Literals::StringLiterals;
GetUnreadSummaryTool::GetUnreadSummaryTool(AccountManager *accountManager)
    : ToolBase(accountManager)
{
}

GetUnreadSummaryTool::~GetUnreadSummaryTool() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolTool GetUnreadSummaryTool::definition() const
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolTool tool = TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::createTool(
        u"get_unread_summary"_s,
        i18n("Get Unread Summary"),
        u"Get a summary of the rooms with unread messages or mentions"_s,
        TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::ToolKind::ReadOnly,
        QMap<QString, QJsonObject>{
            {u"accountName"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Account name (see list_accounts); current account if omitted"_s)}});
    ToolUtils::addToolMeta(tool, ToolUtils::CurrentUser);
    return tool;
}

void GetUnreadSummaryTool::call(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call)
{
    const RocketChatAccount *account = accountFromArguments(call);
    if (!account) {
        call->finishWithText(i18n("No account found"), true);
        return;
    }
    const QList<Room *> &rooms = account->roomModel()->rooms();
    QStringList entries;
    int totalUnread = 0;
    int totalUserMentions = 0;
    int totalGroupMentions = 0;
    for (const Room *room : rooms) {
        if (!room) {
            continue;
        }
        const int unread = room->unread();
        const int userMentions = room->userMentions();
        const int groupMentions = room->groupMentions();
        const qsizetype threadUnread = room->threadUnread().count();
        if (unread <= 0 && userMentions <= 0 && groupMentions <= 0 && threadUnread <= 0) {
            continue;
        }
        totalUnread += unread;
        totalUserMentions += userMentions;
        totalGroupMentions += groupMentions;
        QString entry = u"%1 (id: %2, type: %3) [unread: %4]"_s
                            .arg(room->displayRoomName(), QString::fromLatin1(room->roomId()), Room::roomFromRoomType(room->channelType()))
                            .arg(unread);
        if (userMentions > 0) {
            entry += u" [user mentions: %1]"_s.arg(userMentions);
        }
        if (groupMentions > 0) {
            entry += u" [group mentions: %1]"_s.arg(groupMentions);
        }
        if (threadUnread > 0) {
            entry += u" [unread threads: %1]"_s.arg(threadUnread);
        }
        entries.append(entry);
    }

    if (entries.isEmpty()) {
        call->finishWithText(i18n("No unread messages"));
        return;
    }
    entries.prepend(u"total: %1 room(s), %2 unread message(s), %3 user mention(s), %4 group mention(s)"_s.arg(entries.count())
                        .arg(totalUnread)
                        .arg(totalUserMentions)
                        .arg(totalGroupMentions));
    call->finishWithText(entries.join(u'\n'));
}
