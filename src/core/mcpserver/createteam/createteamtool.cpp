/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "createteamtool.h"
#include "connection.h"
#include "mcpserver/common/toolutils.h"
#include "rocketchataccount.h"
#include "teams/teamscreatejob.h"
#include <KLocalizedString>
#include <QJsonObject>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerToolCall>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>

using namespace Qt::Literals::StringLiterals;
CreateTeamTool::CreateTeamTool(AccountManager *accountManager)
    : ToolBase(accountManager)
{
}

CreateTeamTool::~CreateTeamTool() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolTool CreateTeamTool::definition() const
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolTool tool = TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::createTool(
        u"room_create_team"_s,
        i18n("Create Team"),
        u"Create a new public or private team"_s,
        TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::ToolKind::Modify,
        QMap<QString, QJsonObject>{
            {u"accountName"_s,
             McpProtocolServerTool::schemaProperty(u"string"_s, u"Account name (see currentuser_list_accounts); current account if omitted"_s)},
            {u"name"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Team name"_s)},
            {u"private"_s, McpProtocolServerTool::schemaProperty(u"boolean"_s, u"true to create a private team, false for a public one; false if omitted"_s)},
            {u"members"_s,
             McpProtocolServerTool::schemaArrayProperty(McpProtocolServerTool::schemaProperty(u"string"_s, u"User name (see user_search)"_s),
                                                        u"User names to add to the team"_s)},
            {u"topic"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Team topic"_s)},
            {u"readOnly"_s, McpProtocolServerTool::schemaProperty(u"boolean"_s, u"true if only authorized users can write new messages; false if omitted"_s)}},
        {u"name"_s});
    ToolUtils::addToolMeta(tool, ToolUtils::Rooms);
    return tool;
}

void CreateTeamTool::doCall(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call)
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
    info.infoType = RocketChatRestApi::CreateChannelTeamInfo::CreateInfoType::Team;
    info.name = arguments.value(u"name"_s).toString().trimmed();
    if (info.name.isEmpty()) {
        call->finishWithText(i18n("Missing team name"), true);
        return;
    }
    info.privateChannel = arguments.value(u"private"_s).toBool(false);
    info.members = ToolUtils::userNamesFromArgument(arguments.value(u"members"_s));
    info.topic = arguments.value(u"topic"_s).toString().trimmed();
    info.readOnly = arguments.value(u"readOnly"_s).toBool(false);

    // Job is owned by call: it is deleted when the client cancels the request
    auto job = new RocketChatRestApi::TeamsCreateJob(call);
    account->restApi()->initializeRestApiJob(job);
    job->setTeamsCreateJobInfo(info);
    const QString accountName = account->accountName();
    QObject::connect(job, &RocketChatRestApi::TeamsCreateJob::teamCreateDone, call, [call, accountName, info](const QJsonObject &reply) {
        const QJsonObject team = reply.value("team"_L1).toObject();
        call->finishWithText(u"account: %1\nteam id: %2\nroom id: %3\nteam name: %4\ntype: %5"_s.arg(accountName,
                                                                                                     team.value("_id"_L1).toString(),
                                                                                                     team.value("roomId"_L1).toString(),
                                                                                                     team.value("name"_L1).toString(info.name),
                                                                                                     info.privateChannel ? u"private"_s : u"public"_s));
    });
    QObject::connect(job, &RocketChatRestApi::TeamsCreateJob::failed, call, [call](const QString &serverErrorStr, const QString &descriptionError) {
        call->finishWithText(i18n("Create team failed: %1", descriptionError.isEmpty() ? serverErrorStr : descriptionError), true);
    });
    if (!job->start()) {
        call->finishWithText(i18n("Impossible to start create team job"), true);
    }
}
