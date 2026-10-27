/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "listroomstool.h"
#include "model/roommodel.h"
#include "rocketchataccount.h"
#include "room.h"
#include <KLocalizedString>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerToolCall>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolToolAnnotations>

using namespace Qt::Literals::StringLiterals;
ListRoomsTool::ListRoomsTool(AccountManager *accountManager)
    : ToolBase(accountManager)
{
}

ListRoomsTool::~ListRoomsTool() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolTool ListRoomsTool::definition() const
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolTool tool = TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::createTool(
        u"list_rooms"_s,
        u"List the rooms"_s,
        QMap<QString, QJsonObject>{
            {u"accountName"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Account name (see list_accounts); current account if omitted"_s)}});

    TextAutoGenerateTextMcpProtocolCore::McpProtocolToolAnnotations annotation;
    annotation.setReadOnlyHint(true);
    annotation.setOpenWorldHint(false);
    tool.setTitle(i18n("List Rooms"));
    tool.setAnnotations(annotation);
    // TODO ToolUtils::addToolMeta(tool, ToolUtils::Mail);
    return tool;
}

void ListRoomsTool::call(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call)
{
    const RocketChatAccount *account = accountFromArguments(call);
    if (!account) {
        call->finishWithText(i18n("No account found"), true);
        return;
    }
    const QList<Room *> &rooms = account->roomModel()->rooms();
    QStringList entries;
    entries.reserve(rooms.count());
    for (const Room *room : rooms) {
        if (!room) {
            continue;
        }
        QString entry = u"%1 (display name: %2, id: %3, type: %4)"_s.arg(room->name(),
                                                                         room->displayRoomName(),
                                                                         QString::fromLatin1(room->roomId()),
                                                                         Room::roomFromRoomType(room->channelType()));
        if (room->archived()) {
            entry += u" [archived]"_s;
        }
        if (room->unread() > 0) {
            entry += u" [unread: %1]"_s.arg(room->unread());
        }
        entries.append(entry);
    }

    if (entries.isEmpty()) {
        call->finishWithText(i18n("No rooms found"));
    } else {
        call->finishWithText(entries.join(u'\n'));
    }
}
