/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "getmessagetool.h"
#include "chat/getmessagejob.h"
#include "connection.h"
#include "mcpserver/common/toolutils.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QJsonArray>
#include <QJsonObject>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerToolCall>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>

using namespace Qt::Literals::StringLiterals;
GetMessageTool::GetMessageTool(AccountManager *accountManager)
    : ToolBase(accountManager)
{
}

GetMessageTool::~GetMessageTool() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolTool GetMessageTool::definition() const
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolTool tool = TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::createTool(
        u"message_get"_s,
        i18n("Get Message"),
        u"Get a message from its identifier (see message_search): author, date, room, thread, flags and text"_s,
        TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::ToolKind::ReadOnly,
        QMap<QString, QJsonObject>{
            {u"accountName"_s,
             McpProtocolServerTool::schemaProperty(u"string"_s, u"Account name (see currentuser_list_accounts); current account if omitted"_s)},
            {u"messageId"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Message identifier"_s)}},
        {u"messageId"_s});
    ToolUtils::addToolMeta(tool, ToolUtils::Messages);
    return tool;
}

void GetMessageTool::doCall(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call)
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

    // Job is owned by call: it is deleted when the client cancels the request
    auto job = new RocketChatRestApi::GetMessageJob(call);
    job->setMessageId(messageId.toLatin1());
    account->restApi()->initializeRestApiJob(job);
    const QString userId = QString::fromLatin1(account->userId());
    QObject::connect(job, &RocketChatRestApi::GetMessageJob::getMessageDone, call, [call, userId](const QJsonObject &obj) {
        const QJsonObject message = obj.value("message"_L1).toObject();
        if (message.isEmpty()) {
            call->finishWithText(i18n("Message not found"), true);
            return;
        }
        QStringList entries;
        entries.append(ToolUtils::messageEntry(message, userId));
        entries.append(u"room: %1"_s.arg(message.value("rid"_L1).toString()));
        if (const QString editedAt = message.value("editedAt"_L1).toString(); !editedAt.isEmpty()) {
            entries.append(u"edited: %1"_s.arg(editedAt));
        }
        if (const int threadCount = message.value("tcount"_L1).toInt(); threadCount > 0) {
            entries.append(u"thread replies: %1"_s.arg(threadCount));
        }
        if (const qsizetype attachments = message.value("attachments"_L1).toArray().count(); attachments > 0) {
            entries.append(u"attachments: %1"_s.arg(attachments));
        }
        if (const QJsonObject reactions = message.value("reactions"_L1).toObject(); !reactions.isEmpty()) {
            QStringList reactionList;
            reactionList.reserve(reactions.count());
            for (auto it = reactions.constBegin(), end = reactions.constEnd(); it != end; ++it) {
                reactionList.append(u"%1 (%2)"_s.arg(it.key()).arg(it.value().toObject().value("usernames"_L1).toArray().count()));
            }
            entries.append(u"reactions: %1"_s.arg(reactionList.join(u", "_s)));
        }
        call->finishWithText(entries.join(u'\n'));
    });
    QObject::connect(job, &RocketChatRestApi::GetMessageJob::failed, call, [call](const QString &serverErrorStr, const QString &descriptionError) {
        call->finishWithText(i18n("Get message failed: %1", descriptionError.isEmpty() ? serverErrorStr : descriptionError), true);
    });
    if (!job->start()) {
        call->finishWithText(i18n("Impossible to get message"), true);
    }
}
