/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "searchmessagestool.h"
#include "chat/searchmessagejob.h"
#include "connection.h"
#include "mcpserver/common/toolutils.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QJsonArray>
#include <QJsonObject>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerToolCall>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>

using namespace Qt::Literals::StringLiterals;
namespace
{
constexpr int defaultSearchCount = 25;
constexpr int maximumSearchCount = 100;

[[nodiscard]] bool starredByUser(const QJsonObject &message, const QString &userId)
{
    const QJsonArray starred = message.value("starred"_L1).toArray();
    return std::any_of(starred.cbegin(), starred.cend(), [&userId](const QJsonValue &value) {
        return value.toObject().value("_id"_L1).toString() == userId;
    });
}

[[nodiscard]] QString messageEntry(const QJsonObject &message, const QString &userId)
{
    const QString username = message.value("u"_L1).toObject().value("username"_L1).toString();
    QString entry = u"id: %1, ts: %2, user: %3"_s.arg(message.value("_id"_L1).toString(), message.value("ts"_L1).toString(), username);
    if (const QString threadId = message.value("tmid"_L1).toString(); !threadId.isEmpty()) {
        entry += u", thread: %1"_s.arg(threadId);
    }
    if (message.value("pinned"_L1).toBool()) {
        entry += u" [pinned]";
    }
    if (starredByUser(message, userId)) {
        entry += u" [starred]";
    }
    if (message.value("t"_L1).toString() == "e2e"_L1) {
        entry += u" [encrypted]";
    } else {
        entry += u"\n  text: %1"_s.arg(message.value("msg"_L1).toString());
    }
    return entry;
}
}

SearchMessagesTool::SearchMessagesTool(AccountManager *accountManager)
    : ToolBase(accountManager)
{
}

SearchMessagesTool::~SearchMessagesTool() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolTool SearchMessagesTool::definition() const
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolTool tool = TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::createTool(
        u"search_messages"_s,
        i18n("Search Messages"),
        u"Search messages in a room; returns message identifiers usable with star_message and pin_message"_s,
        TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::ToolKind::ReadOnly,
        QMap<QString, QJsonObject>{
            {u"accountName"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Account name (see list_accounts); current account if omitted"_s)},
            {u"roomId"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Room identifier (see list_rooms or search_rooms)"_s)},
            {u"pattern"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Text to search for"_s)},
            {u"useRegularExpression"_s,
             McpProtocolServerTool::schemaProperty(u"boolean"_s, u"Interpret pattern as a case-insensitive regular expression; false if omitted"_s)},
            {u"count"_s, McpProtocolServerTool::schemaProperty(u"integer"_s, u"Maximum number of results (1-100); 25 if omitted"_s)},
            {u"offset"_s, McpProtocolServerTool::schemaProperty(u"integer"_s, u"Number of results to skip, for paging; 0 if omitted"_s)}},
        {u"roomId"_s, u"pattern"_s});
    ToolUtils::addToolMeta(tool, ToolUtils::Messages);
    return tool;
}

void SearchMessagesTool::doCall(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call)
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
    const auto arguments = call->arguments();
    const QString roomId = arguments.value(u"roomId"_s).toString().trimmed();
    if (roomId.isEmpty()) {
        call->finishWithText(i18n("Missing room identifier"), true);
        return;
    }
    const QString pattern = arguments.value(u"pattern"_s).toString().trimmed();
    if (pattern.isEmpty()) {
        call->finishWithText(i18n("Missing search pattern"), true);
        return;
    }
    const int count = std::clamp(arguments.value(u"count"_s).toInt(defaultSearchCount), 1, maximumSearchCount);
    const int offset = std::max(arguments.value(u"offset"_s).toInt(0), 0);

    // Job is owned by call: it is deleted when the client cancels the request
    auto job = new RocketChatRestApi::SearchMessageJob(call);
    job->setRoomId(roomId);
    job->setSearchText(pattern);
    job->setUseRegularExpression(arguments.value(u"useRegularExpression"_s).toBool(false));
    job->setCount(count);
    job->setOffset(offset);
    account->restApi()->initializeRestApiJob(job);
    const QString userId = QString::fromLatin1(account->userId());
    QObject::connect(job, &RocketChatRestApi::SearchMessageJob::searchMessageDone, call, [call, userId, offset](const QJsonObject &obj) {
        const QJsonArray messages = obj.value("messages"_L1).toArray();
        if (messages.isEmpty()) {
            call->finishWithText(i18n("No messages found"));
            return;
        }
        QStringList entries;
        entries.reserve(messages.count() + 1);
        entries.append(u"found: %1 (offset: %2)"_s.arg(messages.count()).arg(offset));
        for (const QJsonValue &value : messages) {
            entries.append(messageEntry(value.toObject(), userId));
        }
        call->finishWithText(entries.join(u'\n'));
    });
    QObject::connect(job, &RocketChatRestApi::SearchMessageJob::failed, call, [call](const QString &serverErrorStr, const QString &descriptionError) {
        call->finishWithText(i18n("Search failed: %1", descriptionError.isEmpty() ? serverErrorStr : descriptionError), true);
    });
    if (!job->start()) {
        call->finishWithText(i18n("Impossible to start search"), true);
    }
}
