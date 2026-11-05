/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "markroomasreadtool.h"
#include "connection.h"
#include "mcpserver/common/toolutils.h"
#include "model/roommodel.h"
#include "rocketchataccount.h"
#include "room.h"
#include "subscriptions/markroomasreadjob.h"
#include <KLocalizedString>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerToolCall>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>

using namespace Qt::Literals::StringLiterals;
MarkRoomAsReadTool::MarkRoomAsReadTool(AccountManager *accountManager)
    : ToolBase(accountManager)
{
}

MarkRoomAsReadTool::~MarkRoomAsReadTool() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolTool MarkRoomAsReadTool::definition() const
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolTool tool = TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::createTool(
        u"mark_room_as_read"_s,
        i18n("Mark Room as Read"),
        u"Mark all messages of a room as read, including unread threads"_s,
        TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::ToolKind::Modify,
        QMap<QString, QJsonObject>{
            {u"accountName"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Account name (see list_accounts); current account if omitted"_s)},
            {u"roomId"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Room identifier (see list_rooms)"_s)}},
        {u"roomId"_s});
    ToolUtils::addToolMeta(tool, ToolUtils::Rooms);
    return tool;
}

void MarkRoomAsReadTool::call(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call)
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
    const QString roomId = call->arguments().value(u"roomId"_s).toString().trimmed();
    if (roomId.isEmpty()) {
        call->finishWithText(i18n("Missing room identifier"), true);
        return;
    }
    const Room *room = account->roomModel()->findRoom(roomId.toLatin1());
    if (!room) {
        call->finishWithText(i18n("No room found with identifier \"%1\"", roomId), true);
        return;
    }

    // Job is owned by call: it is deleted when the client cancels the request
    auto job = new RocketChatRestApi::MarkRoomAsReadJob(call);
    account->restApi()->initializeRestApiJob(job);
    job->setRoomId(roomId.toLatin1());
    // Forward to Connection so that the account resets the room unread counters
    QObject::connect(job, &RocketChatRestApi::MarkRoomAsReadJob::markAsReadDone, account->restApi(), &Connection::markAsReadDone);
    QObject::connect(job, &RocketChatRestApi::MarkRoomAsReadJob::markAsReadDone, account, &RocketChatAccount::markRoomThreadsAsRead);
    const QString accountName = account->accountName();
    const QString roomName = room->displayRoomName();
    QObject::connect(job, &RocketChatRestApi::MarkRoomAsReadJob::markAsReadDone, call, [call, accountName, roomId, roomName]() {
        call->finishWithText(u"account: %1\nroom id: %2\nroom name: %3\nmarked as read: yes"_s.arg(accountName, roomId, roomName));
    });
    QObject::connect(job, &RocketChatRestApi::MarkRoomAsReadJob::failed, call, [call](const QString &serverErrorStr, const QString &descriptionError) {
        call->finishWithText(i18n("Mark room as read failed: %1", descriptionError.isEmpty() ? serverErrorStr : descriptionError), true);
    });
    if (!job->start()) {
        call->finishWithText(i18n("Impossible to start mark room as read job"), true);
    }
}
