/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "searchroomstool.h"
#include "roominfo/roomsinfo.h"
#include <KLocalizedString>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerToolCall>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>

using namespace Qt::Literals::StringLiterals;
SearchRoomsTool::SearchRoomsTool(AccountManager *accountManager)
    : ToolBase(accountManager)
{
}

SearchRoomsTool::~SearchRoomsTool() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolTool SearchRoomsTool::definition() const
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolTool tool = TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::createTool(
        u"search_rooms"_s,
        i18n("Search Rooms"),
        u"Search the public rooms of the server directory by name, including rooms the user has not joined"_s,
        TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::ToolKind::ReadOnly,
        QMap<QString, QJsonObject>{
            {u"accountName"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Account name (see list_accounts); current account if omitted"_s)},
            {u"pattern"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Text to search in room names (without #)"_s)},
            {u"count"_s, McpProtocolServerTool::schemaProperty(u"integer"_s, u"Maximum number of results (1-100); 25 if omitted"_s)}},
        {u"pattern"_s});
    return tool;
}

void SearchRoomsTool::call(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call)
{
    RocketChatAccount *account = accountFromArguments(call);
    if (!account) {
        call->finishWithText(i18n("No account found"), true);
        return;
    }
    searchDirectory(account, call, RocketChatRestApi::DirectoryJob::SearchType::Rooms, [call](const QJsonObject &obj) {
        RoomsInfo roomsInfo;
        roomsInfo.parseRooms(obj, RoomsInfo::ParseType::Directory);
        if (roomsInfo.isEmpty()) {
            call->finishWithText(i18n("No rooms found"));
            return;
        }
        QStringList entries;
        entries.reserve(roomsInfo.count() + 1);
        entries.append(u"found: %1 (total: %2)"_s.arg(roomsInfo.count()).arg(roomsInfo.total()));
        for (const RoomInfo &roomInfo : roomsInfo.list()) {
            QString entry = u"%1 (id: %2, type: %3, users: %4)"_s.arg(roomInfo.roomName(),
                                                                      QString::fromLatin1(roomInfo.identifier()),
                                                                      roomInfo.channelType(),
                                                                      QString::number(roomInfo.usersCount()));
            if (roomInfo.isTeam()) {
                entry += u" [team]";
            }
            if (const QString belongsTo = roomInfo.belongsTo(); !belongsTo.isEmpty()) {
                entry += u" [belongs to: %1]"_s.arg(belongsTo);
            }
            if (const QString topic = roomInfo.topic(); !topic.isEmpty()) {
                entry += u" topic: %1"_s.arg(topic);
            }
            entries.append(entry);
        }
        call->finishWithText(entries.join(u'\n'));
    });
}
