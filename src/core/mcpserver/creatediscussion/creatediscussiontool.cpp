/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "creatediscussiontool.h"
#include "connection.h"
#include "mcpserver/common/toolutils.h"
#include "model/roommodel.h"
#include "rocketchataccount.h"
#include "room.h"
#include "rooms/roomstartdiscussionjob.h"
#include <KLocalizedString>
#include <QJsonObject>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerToolCall>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>

using namespace Qt::Literals::StringLiterals;
CreateDiscussionTool::CreateDiscussionTool(AccountManager *accountManager)
    : ToolBase(accountManager)
{
}

CreateDiscussionTool::~CreateDiscussionTool() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolTool CreateDiscussionTool::definition() const
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolTool tool = TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::createTool(
        u"room_create_discussion"_s,
        i18n("Create Discussion"),
        u"Create a discussion inside a room, optionally started from a message of that room"_s,
        TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::ToolKind::Modify,
        QMap<QString, QJsonObject>{
            {u"accountName"_s,
             McpProtocolServerTool::schemaProperty(u"string"_s, u"Account name (see currentuser_list_accounts); current account if omitted"_s)},
            {u"parentRoomId"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Identifier of the room containing the discussion (see room_list)"_s)},
            {u"name"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Discussion name"_s)},
            {u"parentMessageId"_s,
             McpProtocolServerTool::schemaProperty(u"string"_s, u"Identifier of the message the discussion starts from (see message_search)"_s)},
            {u"message"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"First message posted in the discussion"_s)},
            {u"topic"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Discussion topic"_s)},
            {u"members"_s,
             McpProtocolServerTool::schemaArrayProperty(McpProtocolServerTool::schemaProperty(u"string"_s, u"User name (see user_search)"_s),
                                                        u"User names to invite to the discussion"_s)}},
        {u"parentRoomId"_s, u"name"_s});
    ToolUtils::addToolMeta(tool, ToolUtils::Rooms);
    return tool;
}

void CreateDiscussionTool::doCall(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call)
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
    const QMap<QString, QJsonValue> arguments = call->arguments();
    const QString parentRoomId = arguments.value(u"parentRoomId"_s).toString().trimmed();
    if (parentRoomId.isEmpty()) {
        call->finishWithText(i18n("Missing parent room identifier"), true);
        return;
    }
    const Room *room = account->roomModel()->findRoom(parentRoomId.toLatin1());
    if (!room) {
        call->finishWithText(i18n("No room found with identifier \"%1\"", parentRoomId), true);
        return;
    }
    const QString name = arguments.value(u"name"_s).toString().trimmed();
    if (name.isEmpty()) {
        call->finishWithText(i18n("Missing discussion name"), true);
        return;
    }
    const QStringList members = ToolUtils::userNamesFromArgument(arguments.value(u"members"_s));
    QList<QByteArray> users;
    users.reserve(members.count());
    for (const QString &member : members) {
        users.append(member.toUtf8());
    }

    // Job is owned by call: it is deleted when the client cancels the request
    auto job = new RocketChatRestApi::RoomStartDiscussionJob(call);
    account->restApi()->initializeRestApiJob(job);
    job->setParentRoomId(parentRoomId.toLatin1());
    job->setDiscussionName(name);
    job->setParentMessageId(arguments.value(u"parentMessageId"_s).toString().trimmed().toLatin1());
    job->setReplyMessage(arguments.value(u"message"_s).toString());
    job->setTopic(arguments.value(u"topic"_s).toString().trimmed());
    job->setUsers(users);
    const QString accountName = account->accountName();
    const QString parentRoomName = room->displayRoomName();
    QObject::connect(job,
                     &RocketChatRestApi::RoomStartDiscussionJob::startDiscussionDone,
                     call,
                     [call, accountName, parentRoomId, parentRoomName, name](const QJsonObject &reply) {
                         const QJsonObject discussion = reply.value("discussion"_L1).toObject();
                         call->finishWithText(u"account: %1\ndiscussion id: %2\ndiscussion name: %3\nparent room id: %4\nparent room name: %5"_s.arg(
                             accountName,
                             discussion.value("_id"_L1).toString(),
                             discussion.value("fname"_L1).toString(name),
                             parentRoomId,
                             parentRoomName));
                     });
    QObject::connect(job, &RocketChatRestApi::RoomStartDiscussionJob::failed, call, [call](const QString &serverErrorStr, const QString &descriptionError) {
        call->finishWithText(i18n("Create discussion failed: %1", descriptionError.isEmpty() ? serverErrorStr : descriptionError), true);
    });
    if (!job->start()) {
        call->finishWithText(i18n("Impossible to start create discussion job"), true);
    }
}
