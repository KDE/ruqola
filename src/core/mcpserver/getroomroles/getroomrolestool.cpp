/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "getroomrolestool.h"
#include "channels/getchannelrolesjob.h"
#include "connection.h"
#include "groups/getgrouprolesjob.h"
#include "mcpserver/common/toolutils.h"
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
[[nodiscard]] QString roleEntry(const QJsonObject &obj)
{
    const QJsonObject user = obj.value("u"_L1).toObject();
    QString entry = user.value("username"_L1).toString();
    const QString userId = user.value("_id"_L1).toString();
    if (const QString name = user.value("name"_L1).toString(); !name.isEmpty()) {
        entry += u" (name: %1, id: %2)"_s.arg(name, userId);
    } else {
        entry += u" (id: %1)"_s.arg(userId);
    }
    QStringList roles;
    const QJsonArray roleArray = obj.value("roles"_L1).toArray();
    roles.reserve(roleArray.count());
    for (const auto &role : roleArray) {
        roles.append(role.toString());
    }
    entry += u": "_s + roles.join(u", "_s);
    return entry;
}

void finishWithRoles(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call, const QString &header, const QJsonObject &obj)
{
    const QJsonArray roles = obj.value("roles"_L1).toArray();
    if (roles.isEmpty()) {
        call->finishWithText(header + u"\n"_s + i18n("No users with roles found"));
        return;
    }
    QStringList entries;
    entries.reserve(roles.count() + 2);
    entries.append(header);
    entries.append(u"found: %1"_s.arg(roles.count()));
    for (const auto &value : roles) {
        entries.append(roleEntry(value.toObject()));
    }
    call->finishWithText(entries.join(u'\n'));
}
}

GetRoomRolesTool::GetRoomRolesTool(AccountManager *accountManager)
    : ToolBase(accountManager)
{
}

GetRoomRolesTool::~GetRoomRolesTool() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolTool GetRoomRolesTool::definition() const
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolTool tool = TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::createTool(
        u"get_room_roles"_s,
        i18n("Get Room Roles"),
        u"List the users who have a role (owner, moderator, leader) in a channel or private group"_s,
        TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::ToolKind::ReadOnly,
        QMap<QString, QJsonObject>{
            {u"accountName"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Account name (see list_accounts); current account if omitted"_s)},
            {u"roomId"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Room identifier (see list_rooms or search_rooms)"_s)}},
        {u"roomId"_s});
    ToolUtils::addToolMeta(tool, ToolUtils::Rooms);
    return tool;
}

void GetRoomRolesTool::call(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call)
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
    const QString roomId = call->arguments().value(u"roomId"_s).toString().trimmed();
    if (roomId.isEmpty()) {
        call->finishWithText(i18n("Missing room identifier"), true);
        return;
    }
    const Room *room = account->roomModel()->findRoom(roomId.toLatin1());
    if (!room) {
        call->finishWithText(i18n("No room found with identifier \"%1\"", roomId), true);
        return;
    }
    const QString header = u"account: %1\nroom id: %2\nroom name: %3"_s.arg(account->accountName(), roomId, room->displayRoomName());

    RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfoType::RoomIdentifier;
    info.identifier = roomId;

    // Jobs are owned by call: they are deleted when the client cancels the request
    RocketChatRestApi::ChannelGroupBaseJob *job = nullptr;
    switch (room->channelType()) {
    case Room::RoomType::Channel: {
        auto channelJob = new RocketChatRestApi::GetChannelRolesJob(call);
        QObject::connect(channelJob, &RocketChatRestApi::GetChannelRolesJob::channelRolesDone, call, [call, header](const QJsonObject &obj) {
            finishWithRoles(call, header, obj);
        });
        job = channelJob;
        break;
    }
    case Room::RoomType::Private: {
        auto groupJob = new RocketChatRestApi::GetGroupRolesJob(call);
        QObject::connect(groupJob, &RocketChatRestApi::GetGroupRolesJob::groupRolesDone, call, [call, header](const QJsonObject &obj) {
            finishWithRoles(call, header, obj);
        });
        job = groupJob;
        break;
    }
    case Room::RoomType::Direct:
        call->finishWithText(i18n("Direct conversations have no room roles"), true);
        return;
    case Room::RoomType::Unknown:
        call->finishWithText(i18n("Unknown room type"), true);
        return;
    }
    job->setChannelGroupInfo(info);
    account->restApi()->initializeRestApiJob(job);
    QObject::connect(job, &RocketChatRestApi::RestApiAbstractJob::failed, call, [call](const QString &serverErrorStr, const QString &descriptionError) {
        call->finishWithText(i18n("Getting room roles failed: %1", descriptionError.isEmpty() ? serverErrorStr : descriptionError), true);
    });
    if (!job->start()) {
        call->finishWithText(i18n("Impossible to start getting room roles"), true);
    }
}
