/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "getroominfotool.h"
#include "model/roommodel.h"
#include "rocketchataccount.h"
#include "room.h"
#include <KLocalizedString>
#include <QDateTime>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerToolCall>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolToolAnnotations>

using namespace Qt::Literals::StringLiterals;
GetRoomInfoTool::GetRoomInfoTool(AccountManager *accountManager)
    : ToolBase(accountManager)
{
}

GetRoomInfoTool::~GetRoomInfoTool() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolTool GetRoomInfoTool::definition() const
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolTool tool = TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::createTool(
        u"get_room_info"_s,
        u"Get information about a room"_s,
        QMap<QString, QJsonObject>{
            {u"accountName"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Account name (see list_accounts); current account if omitted"_s)},
            {u"roomId"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Room identifier (see list_rooms)"_s)}},
        {u"roomId"_s});

    TextAutoGenerateTextMcpProtocolCore::McpProtocolToolAnnotations annotation;
    annotation.setReadOnlyHint(true);
    annotation.setOpenWorldHint(false);
    tool.setTitle(i18n("Get Room Info"));
    tool.setAnnotations(annotation);
    return tool;
}

void GetRoomInfoTool::call(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call)
{
    const RocketChatAccount *account = accountFromArguments(call);
    if (!account) {
        call->finishWithText(i18n("No account found"), true);
        return;
    }
    const QString roomId = call->arguments().value(u"roomId"_s).toString();
    if (roomId.isEmpty()) {
        call->finishWithText(i18n("Missing room identifier"), true);
        return;
    }
    const Room *room = account->roomModel()->findRoom(roomId.toLatin1());
    if (!room) {
        call->finishWithText(i18n("No room found with identifier \"%1\"", roomId), true);
        return;
    }

    QStringList entries;
    entries.append(u"name: %1"_s.arg(room->name()));
    entries.append(u"display name: %1"_s.arg(room->displayRoomName()));
    entries.append(u"id: %1"_s.arg(QString::fromLatin1(room->roomId())));
    entries.append(u"type: %1"_s.arg(Room::roomFromRoomType(room->channelType())));
    if (const QString owner = room->roomOwnerUserName(); !owner.isEmpty()) {
        entries.append(u"owner: %1"_s.arg(owner));
    }
    if (const QString topic = room->topic(); !topic.isEmpty()) {
        entries.append(u"topic: %1"_s.arg(topic));
    }
    if (const QString announcement = room->announcement(); !announcement.isEmpty()) {
        entries.append(u"announcement: %1"_s.arg(announcement));
    }
    if (const QString description = room->description(); !description.isEmpty()) {
        entries.append(u"description: %1"_s.arg(description));
    }
    if (room->isDiscussionRoom()) {
        entries.append(u"parent room id: %1"_s.arg(QString::fromLatin1(room->parentRid())));
    }
    entries.append(u"unread: %1"_s.arg(room->unread()));
    entries.append(u"user mentions: %1"_s.arg(room->userMentions()));
    entries.append(u"group mentions: %1"_s.arg(room->groupMentions()));
    if (const qint64 numberMessages = room->numberMessages(); numberMessages > 0) {
        entries.append(u"number of messages: %1"_s.arg(numberMessages));
    }
    if (const qint64 lastMessageAt = room->lastMessageAt(); lastMessageAt > 0) {
        entries.append(u"last message at: %1"_s.arg(QDateTime::fromMSecsSinceEpoch(lastMessageAt).toString(Qt::ISODate)));
    }
    entries.append(u"favorite: %1"_s.arg(room->favorite() ? u"yes"_s : u"no"_s));
    entries.append(u"read only: %1"_s.arg(room->readOnly() ? u"yes"_s : u"no"_s));
    entries.append(u"archived: %1"_s.arg(room->archived() ? u"yes"_s : u"no"_s));
    entries.append(u"broadcast: %1"_s.arg(room->broadcast() ? u"yes"_s : u"no"_s));
    entries.append(u"encrypted: %1"_s.arg(room->encrypted() ? u"yes"_s : u"no"_s));

    call->finishWithText(entries.join(u'\n'));
}
