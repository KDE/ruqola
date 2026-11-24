/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "createroomtool.h"
#include "channels/createchanneljob.h"
#include "connection.h"
#include "groups/creategroupsjob.h"
#include "mcpserver/common/toolutils.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QJsonObject>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerToolCall>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>

using namespace Qt::Literals::StringLiterals;
CreateRoomTool::CreateRoomTool(AccountManager *accountManager)
    : ToolBase(accountManager)
{
}

CreateRoomTool::~CreateRoomTool() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolTool CreateRoomTool::definition() const
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolTool tool = TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::createTool(
        u"room_create"_s,
        i18n("Create Room"),
        u"Create a new public or private room (channel)"_s,
        TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::ToolKind::Modify,
        QMap<QString, QJsonObject>{
            {u"accountName"_s,
             McpProtocolServerTool::schemaProperty(u"string"_s, u"Account name (see currentuser_list_accounts); current account if omitted"_s)},
            {u"name"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Room name"_s)},
            {u"private"_s, McpProtocolServerTool::schemaProperty(u"boolean"_s, u"true to create a private room, false for a public one; false if omitted"_s)},
            {u"members"_s,
             McpProtocolServerTool::schemaArrayProperty(McpProtocolServerTool::schemaProperty(u"string"_s, u"User name (see user_search)"_s),
                                                        u"User names to add to the room"_s)},
            {u"topic"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Room topic"_s)},
            {u"readOnly"_s, McpProtocolServerTool::schemaProperty(u"boolean"_s, u"true if only authorized users can write new messages; false if omitted"_s)}},
        {u"name"_s});
    ToolUtils::addToolMeta(tool, ToolUtils::Rooms);
    return tool;
}

void CreateRoomTool::doCall(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call)
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
    RocketChatRestApi::CreateChannelTeamInfo info;
    info.infoType = RocketChatRestApi::CreateChannelTeamInfo::CreateInfoType::Channel;
    info.name = arguments.value(u"name"_s).toString().trimmed();
    if (info.name.isEmpty()) {
        call->finishWithText(i18n("Missing room name"), true);
        return;
    }
    info.privateChannel = arguments.value(u"private"_s).toBool(false);
    info.members = ToolUtils::userNamesFromArgument(arguments.value(u"members"_s));
    info.topic = arguments.value(u"topic"_s).toString().trimmed();
    info.readOnly = arguments.value(u"readOnly"_s).toBool(false);

    const QString accountName = account->accountName();
    const auto finished = [call, accountName, info](const QJsonObject &room) {
        call->finishWithText(u"account: %1\nroom id: %2\nroom name: %3\ntype: %4"_s.arg(accountName,
                                                                                        room.value("_id"_L1).toString(),
                                                                                        room.value("name"_L1).toString(info.name),
                                                                                        info.privateChannel ? u"private"_s : u"public"_s));
    };
    const auto failed = [call](const QString &serverErrorStr, const QString &descriptionError) {
        call->finishWithText(i18n("Create room failed: %1", descriptionError.isEmpty() ? serverErrorStr : descriptionError), true);
    };

    // Job is owned by call: it is deleted when the client cancels the request
    if (info.privateChannel) {
        auto job = new RocketChatRestApi::CreateGroupsJob(call);
        account->restApi()->initializeRestApiJob(job);
        job->setCreateGroupsInfo(info);
        QObject::connect(job, &RocketChatRestApi::CreateGroupsJob::createGroupsDone, call, [finished](const QJsonObject &reply) {
            finished(reply.value("group"_L1).toObject());
        });
        QObject::connect(job, &RocketChatRestApi::CreateGroupsJob::failed, call, failed);
        if (!job->start()) {
            call->finishWithText(i18n("Impossible to start create room job"), true);
        }
    } else {
        auto job = new RocketChatRestApi::CreateChannelJob(call);
        account->restApi()->initializeRestApiJob(job);
        job->setCreateChannelInfo(info);
        QObject::connect(job, &RocketChatRestApi::CreateChannelJob::createChannelDone, call, [finished](const QJsonObject &reply) {
            finished(reply.value("channel"_L1).toObject());
        });
        QObject::connect(job, &RocketChatRestApi::CreateChannelJob::failed, call, failed);
        if (!job->start()) {
            call->finishWithText(i18n("Impossible to start create room job"), true);
        }
    }
}
