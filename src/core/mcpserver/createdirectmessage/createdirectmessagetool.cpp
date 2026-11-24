/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "createdirectmessagetool.h"
#include "connection.h"
#include "directmessage/createdmjob.h"
#include "mcpserver/common/toolutils.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QJsonObject>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerToolCall>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>

using namespace Qt::Literals::StringLiterals;
CreateDirectMessageTool::CreateDirectMessageTool(AccountManager *accountManager)
    : ToolBase(accountManager)
{
}

CreateDirectMessageTool::~CreateDirectMessageTool() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolTool CreateDirectMessageTool::definition() const
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolTool tool = TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::createTool(
        u"room_create_direct_message"_s,
        i18n("Create Direct Message"),
        u"Open a direct message room with one or more users (several users create a group direct message). The existing room is returned if it already exists"_s,
        TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::ToolKind::Modify,
        QMap<QString, QJsonObject>{
            {u"accountName"_s,
             McpProtocolServerTool::schemaProperty(u"string"_s, u"Account name (see currentuser_list_accounts); current account if omitted"_s)},
            {u"userNames"_s,
             McpProtocolServerTool::schemaArrayProperty(McpProtocolServerTool::schemaProperty(u"string"_s, u"User name (see user_search)"_s),
                                                        u"User names to talk with, without the current user"_s)}},
        {u"userNames"_s});
    ToolUtils::addToolMeta(tool, ToolUtils::Rooms);
    return tool;
}

void CreateDirectMessageTool::doCall(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call)
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
    const QStringList userNames = ToolUtils::userNamesFromArgument(call->arguments().value(u"userNames"_s));
    if (userNames.isEmpty()) {
        call->finishWithText(i18n("Missing user names"), true);
        return;
    }

    // Job is owned by call: it is deleted when the client cancels the request
    auto job = new RocketChatRestApi::CreateDmJob(call);
    account->restApi()->initializeRestApiJob(job);
    job->setUserNames(userNames);
    const QString accountName = account->accountName();
    QObject::connect(job, &RocketChatRestApi::CreateDmJob::createDmDone, call, [call, accountName, userNames](const QJsonObject &reply) {
        const QJsonObject room = reply.value("room"_L1).toObject();
        const QString roomId = room.value("rid"_L1).toString(room.value("_id"_L1).toString());
        call->finishWithText(u"account: %1\nroom id: %2\nusers: %3"_s.arg(accountName, roomId, userNames.join(u", "_s)));
    });
    QObject::connect(job, &RocketChatRestApi::CreateDmJob::failed, call, [call](const QString &serverErrorStr, const QString &descriptionError) {
        call->finishWithText(i18n("Create direct message failed: %1", descriptionError.isEmpty() ? serverErrorStr : descriptionError), true);
    });
    if (!job->start()) {
        call->finishWithText(i18n("Impossible to start create direct message job"), true);
    }
}
