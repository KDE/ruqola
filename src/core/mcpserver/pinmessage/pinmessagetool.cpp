/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "pinmessagetool.h"
#include "chat/pinmessagejob.h"
#include "connection.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerToolCall>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>

using namespace Qt::Literals::StringLiterals;
PinMessageTool::PinMessageTool(AccountManager *accountManager)
    : ToolBase(accountManager)
{
}

PinMessageTool::~PinMessageTool() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolTool PinMessageTool::definition() const
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolTool tool = TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::createTool(
        u"pin_message"_s,
        i18n("Pin Message"),
        u"Pin or unpin a message in its room"_s,
        TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::ToolKind::Modify,
        QMap<QString, QJsonObject>{
            {u"accountName"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Account name (see list_accounts); current account if omitted"_s)},
            {u"messageId"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Message identifier"_s)},
            {u"pinned"_s, McpProtocolServerTool::schemaProperty(u"boolean"_s, u"true to pin the message, false to unpin it; true if omitted"_s)}},
        {u"messageId"_s});
    return tool;
}

void PinMessageTool::call(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call)
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
    const bool pinned = call->arguments().value(u"pinned"_s).toBool(true);

    // Job is owned by call: it is deleted when the client cancels the request
    auto job = new RocketChatRestApi::PinMessageJob(call);
    account->restApi()->initializeRestApiJob(job);
    job->setMessageId(messageId.toLatin1());
    job->setPinMessage(pinned);
    const QString accountName = account->accountName();
    QObject::connect(job, &RocketChatRestApi::PinMessageJob::pinMessageDone, call, [call, accountName, messageId]() {
        call->finishWithText(u"account: %1\nmessage id: %2\npinned: yes"_s.arg(accountName, messageId));
    });
    QObject::connect(job, &RocketChatRestApi::PinMessageJob::unPinMessageDone, call, [call, accountName, messageId]() {
        call->finishWithText(u"account: %1\nmessage id: %2\npinned: no"_s.arg(accountName, messageId));
    });
    QObject::connect(job, &RocketChatRestApi::PinMessageJob::failed, call, [call](const QString &serverErrorStr, const QString &descriptionError) {
        call->finishWithText(i18n("Pin message failed: %1", descriptionError.isEmpty() ? serverErrorStr : descriptionError), true);
    });
    if (!job->start()) {
        call->finishWithText(i18n("Impossible to start pin message job"), true);
    }
}
