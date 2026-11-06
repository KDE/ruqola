/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "starmessagetool.h"
#include "chat/starmessagejob.h"
#include "connection.h"
#include "mcpserver/common/toolutils.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerToolCall>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>

using namespace Qt::Literals::StringLiterals;
StarMessageTool::StarMessageTool(AccountManager *accountManager)
    : ToolBase(accountManager)
{
}

StarMessageTool::~StarMessageTool() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolTool StarMessageTool::definition() const
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolTool tool = TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::createTool(
        u"star_message"_s,
        i18n("Star Message"),
        u"Star (mark as favorite) or unstar a message"_s,
        TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::ToolKind::Modify,
        QMap<QString, QJsonObject>{
            {u"accountName"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Account name (see list_accounts); current account if omitted"_s)},
            {u"messageId"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Message identifier"_s)},
            {u"starred"_s, McpProtocolServerTool::schemaProperty(u"boolean"_s, u"true to star the message, false to unstar it; true if omitted"_s)}},
        {u"messageId"_s});
    ToolUtils::addToolMeta(tool, ToolUtils::Messages);
    return tool;
}

void StarMessageTool::doCall(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call)
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
    const QString messageId = call->arguments().value(u"messageId"_s).toString().trimmed();
    if (messageId.isEmpty()) {
        call->finishWithText(i18n("Missing message identifier"), true);
        return;
    }
    const bool starred = call->arguments().value(u"starred"_s).toBool(true);

    // Job is owned by call: it is deleted when the client cancels the request
    auto job = new RocketChatRestApi::StarMessageJob(call);
    account->restApi()->initializeRestApiJob(job);
    job->setMessageId(messageId.toLatin1());
    job->setStarMessage(starred);
    const QString accountName = account->accountName();
    QObject::connect(job, &RocketChatRestApi::StarMessageJob::messageStarred, call, [call, accountName, messageId, starred]() {
        call->finishWithText(u"account: %1\nmessage id: %2\nstarred: %3"_s.arg(accountName, messageId, starred ? u"yes"_s : u"no"_s));
    });
    QObject::connect(job, &RocketChatRestApi::StarMessageJob::failed, call, [call](const QString &serverErrorStr, const QString &descriptionError) {
        call->finishWithText(i18n("Star message failed: %1", descriptionError.isEmpty() ? serverErrorStr : descriptionError), true);
    });
    if (!job->start()) {
        call->finishWithText(i18n("Impossible to start star message job"), true);
    }
}
