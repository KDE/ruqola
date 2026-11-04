/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "listroommemberstool.h"
#include "channels/channelmembersjob.h"
#include "connection.h"
#include "model/roommodel.h"
#include "rocketchataccount.h"
#include "room.h"
#include <KLocalizedString>
#include <QJsonArray>
#include <QJsonObject>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerToolCall>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>

using namespace Qt::Literals::StringLiterals;
namespace
{
constexpr int defaultMembersCount = 50;
constexpr int maximumMembersCount = 100;

[[nodiscard]] QString memberEntry(const QJsonObject &member)
{
    QString entry = member.value("username"_L1).toString();
    const QString userId = member.value("_id"_L1).toString();
    if (const QString name = member.value("name"_L1).toString(); !name.isEmpty()) {
        entry += u" (name: %1, id: %2"_s.arg(name, userId);
    } else {
        entry += u" (id: %1"_s.arg(userId);
    }
    if (const QString status = member.value("status"_L1).toString(); !status.isEmpty()) {
        entry += u", status: %1"_s.arg(status);
    }
    entry += u')';
    return entry;
}
}

ListRoomMembersTool::ListRoomMembersTool(AccountManager *accountManager)
    : ToolBase(accountManager)
{
}

ListRoomMembersTool::~ListRoomMembersTool() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolTool ListRoomMembersTool::definition() const
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolTool tool = TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::createTool(
        u"list_room_members"_s,
        i18n("List Room Members"),
        u"List the members of a room with their user name, name, identifier and presence status"_s,
        TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::ToolKind::ReadOnly,
        QMap<QString, QJsonObject>{
            {u"accountName"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Account name (see list_accounts); current account if omitted"_s)},
            {u"roomId"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Room identifier (see list_rooms or search_rooms)"_s)},
            {u"filter"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Only return members whose user name or name contains this text"_s)},
            {u"count"_s, McpProtocolServerTool::schemaProperty(u"integer"_s, u"Maximum number of results (1-100); 50 if omitted"_s)},
            {u"offset"_s, McpProtocolServerTool::schemaProperty(u"integer"_s, u"Number of results to skip, for paging; 0 if omitted"_s)}},
        {u"roomId"_s});
    return tool;
}

void ListRoomMembersTool::call(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call)
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
    const Room *room = account->roomModel()->findRoom(roomId.toLatin1());
    if (!room) {
        call->finishWithText(i18n("No room found with identifier \"%1\"", roomId), true);
        return;
    }
    RocketChatRestApi::ChannelMembersJob::ChannelType channelType = RocketChatRestApi::ChannelMembersJob::ChannelType::Unknown;
    switch (room->channelType()) {
    case Room::RoomType::Channel:
        channelType = RocketChatRestApi::ChannelMembersJob::ChannelType::Channel;
        break;
    case Room::RoomType::Private:
        channelType = RocketChatRestApi::ChannelMembersJob::ChannelType::Groups;
        break;
    case Room::RoomType::Direct:
        channelType = RocketChatRestApi::ChannelMembersJob::ChannelType::Direct;
        break;
    case Room::RoomType::Unknown:
        call->finishWithText(i18n("Unknown room type"), true);
        return;
    }
    const int count = std::clamp(arguments.value(u"count"_s).toInt(defaultMembersCount), 1, maximumMembersCount);
    const int offset = std::max(arguments.value(u"offset"_s).toInt(0), 0);

    // Job is owned by call: it is deleted when the client cancels the request
    auto job = new RocketChatRestApi::ChannelMembersJob(call);
    RocketChatRestApi::QueryParameters parameters;
    parameters.setCount(count);
    parameters.setOffset(offset);
    if (const QString filter = arguments.value(u"filter"_s).toString().trimmed(); !filter.isEmpty()) {
        parameters.setFilter(filter);
    }
    job->setQueryParameters(parameters);
    RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfoType::RoomIdentifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    job->setChannelType(channelType);
    account->restApi()->initializeRestApiJob(job);
    QObject::connect(job, &RocketChatRestApi::ChannelMembersJob::channelMembersDone, call, [call](const QJsonObject &obj) {
        const QJsonArray members = obj.value("members"_L1).toArray();
        if (members.isEmpty()) {
            call->finishWithText(i18n("No members found"));
            return;
        }
        QStringList entries;
        entries.reserve(members.count() + 1);
        entries.append(u"found: %1 (offset: %2, total: %3)"_s.arg(members.count()).arg(obj.value("offset"_L1).toInt()).arg(obj.value("total"_L1).toInt()));
        for (const auto &value : members) {
            entries.append(memberEntry(value.toObject()));
        }
        call->finishWithText(entries.join(u'\n'));
    });
    QObject::connect(job, &RocketChatRestApi::ChannelMembersJob::failed, call, [call](const QString &serverErrorStr, const QString &descriptionError) {
        call->finishWithText(i18n("Listing members failed: %1", descriptionError.isEmpty() ? serverErrorStr : descriptionError), true);
    });
    if (!job->start()) {
        call->finishWithText(i18n("Impossible to start listing members"), true);
    }
}
