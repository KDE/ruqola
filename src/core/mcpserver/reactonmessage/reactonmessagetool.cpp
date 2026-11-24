/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "reactonmessagetool.h"
#include "chat/reactonmessagejob.h"
#include "connection.h"
#include "mcpserver/common/toolutils.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerToolCall>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>

using namespace Qt::Literals::StringLiterals;
ReactOnMessageTool::ReactOnMessageTool(AccountManager *accountManager)
    : ToolBase(accountManager)
{
}

ReactOnMessageTool::~ReactOnMessageTool() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolTool ReactOnMessageTool::definition() const
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolTool tool = TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::createTool(
        u"message_reacton"_s,
        i18n("React on Message"),
        u"Add or remove an emoji reaction on a message"_s,
        TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::ToolKind::Modify,
        QMap<QString, QJsonObject>{
            {u"accountName"_s,
             McpProtocolServerTool::schemaProperty(u"string"_s, u"Account name (see currentuser_list_accounts); current account if omitted"_s)},
            {u"messageId"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Message identifier (see message_search)"_s)},
            {u"emoji"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Emoji short name, with or without colons (e.g. :thumbsup: or thumbsup)"_s)},
            {u"shouldReact"_s, McpProtocolServerTool::schemaProperty(u"boolean"_s, u"true to add the reaction, false to remove it; true if omitted"_s)}},
        {u"messageId"_s, u"emoji"_s});
    ToolUtils::addToolMeta(tool, ToolUtils::Messages);
    return tool;
}

void ReactOnMessageTool::doCall(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call)
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
    QString emoji = call->arguments().value(u"emoji"_s).toString().trimmed();
    if (emoji.startsWith(u':')) {
        emoji.remove(0, 1);
    }
    if (emoji.endsWith(u':')) {
        emoji.chop(1);
    }
    if (emoji.isEmpty()) {
        call->finishWithText(i18n("Missing emoji"), true);
        return;
    }
    const bool shouldReact = call->arguments().value(u"shouldReact"_s).toBool(true);

    // Job is owned by call: it is deleted when the client cancels the request
    auto job = new RocketChatRestApi::ReactOnMessageJob(call);
    account->restApi()->initializeRestApiJob(job);
    job->setMessageId(messageId.toLatin1());
    job->setEmoji(emoji);
    job->setShouldReact(shouldReact);
    const QString accountName = account->accountName();
    QObject::connect(job, &RocketChatRestApi::ReactOnMessageJob::reactOnMessageDone, call, [call, accountName, messageId, emoji, shouldReact]() {
        call->finishWithText(
            u"account: %1\nmessage id: %2\nemoji: :%3:\nreaction: %4"_s.arg(accountName, messageId, emoji, shouldReact ? u"added"_s : u"removed"_s));
    });
    QObject::connect(job, &RocketChatRestApi::ReactOnMessageJob::failed, call, [call](const QString &serverErrorStr, const QString &descriptionError) {
        call->finishWithText(i18n("React on message failed: %1", descriptionError.isEmpty() ? serverErrorStr : descriptionError), true);
    });
    if (!job->start()) {
        call->finishWithText(i18n("Impossible to start react on message job"), true);
    }
}
