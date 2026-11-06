/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "updateroomsettingstool.h"
#include "connection.h"
#include "mcpserver/common/toolutils.h"
#include "model/roommodel.h"
#include "rocketchataccount.h"
#include "room.h"
#include "rooms/saveroomsettingsjob.h"
#include <KLocalizedString>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerToolCall>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>

using namespace Qt::Literals::StringLiterals;
using SaveRoomSettingsInfo = RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo;
UpdateRoomSettingsTool::UpdateRoomSettingsTool(AccountManager *accountManager)
    : ToolBase(accountManager)
{
}

UpdateRoomSettingsTool::~UpdateRoomSettingsTool() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolTool UpdateRoomSettingsTool::definition() const
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolTool tool = TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::createTool(
        u"update_room_settings"_s,
        i18n("Update Room Settings"),
        u"Change the name, topic, description or announcement of a room. Only the given fields are changed; pass an empty string to clear a field"_s,
        TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::ToolKind::Modify,
        QMap<QString, QJsonObject>{
            {u"accountName"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Account name (see list_accounts); current account if omitted"_s)},
            {u"roomId"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Room identifier (see list_rooms)"_s)},
            {u"name"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"New room name (cannot be empty)"_s)},
            {u"topic"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"New room topic"_s)},
            {u"description"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"New room description"_s)},
            {u"announcement"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"New room announcement"_s)}},
        {u"roomId"_s});
    ToolUtils::addToolMeta(tool, ToolUtils::Rooms);
    return tool;
}

void UpdateRoomSettingsTool::doCall(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call)
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
    if (room->channelType() == Room::RoomType::Direct) {
        call->finishWithText(i18n("Settings of a direct room cannot be changed"), true);
        return;
    }

    SaveRoomSettingsInfo info;
    info.roomId = roomId.toLatin1();
    info.roomType = room->roomFromRoomType(room->channelType());
    QStringList changes;
    if (arguments.contains(u"name"_s)) {
        const QString name = arguments.value(u"name"_s).toString().trimmed();
        if (name.isEmpty()) {
            call->finishWithText(i18n("Room name cannot be empty"), true);
            return;
        }
        info.roomName = name;
        info.mSettingsWillBeChanged |= SaveRoomSettingsInfo::RoomName;
        changes.append(u"name: %1"_s.arg(name));
    }
    if (arguments.contains(u"topic"_s)) {
        info.roomTopic = arguments.value(u"topic"_s).toString();
        info.mSettingsWillBeChanged |= SaveRoomSettingsInfo::RoomTopic;
        changes.append(u"topic: %1"_s.arg(info.roomTopic));
    }
    if (arguments.contains(u"description"_s)) {
        info.roomDescription = arguments.value(u"description"_s).toString();
        info.mSettingsWillBeChanged |= SaveRoomSettingsInfo::RoomDescription;
        changes.append(u"description: %1"_s.arg(info.roomDescription));
    }
    if (arguments.contains(u"announcement"_s)) {
        info.roomAnnouncement = arguments.value(u"announcement"_s).toString();
        info.mSettingsWillBeChanged |= SaveRoomSettingsInfo::RoomAnnouncement;
        changes.append(u"announcement: %1"_s.arg(info.roomAnnouncement));
    }
    if (changes.isEmpty()) {
        call->finishWithText(i18n("Nothing to change: give at least one of name, topic, description or announcement"), true);
        return;
    }

    // Job is owned by call: it is deleted when the client cancels the request
    auto job = new RocketChatRestApi::SaveRoomSettingsJob(call);
    account->restApi()->initializeRestApiJob(job);
    job->setSaveRoomSettingsInfo(info);
    const QString accountName = account->accountName();
    const QString roomName = room->displayRoomName();
    QObject::connect(job, &RocketChatRestApi::SaveRoomSettingsJob::saveRoomSettingsDone, call, [call, accountName, roomId, roomName, changes]() {
        call->finishWithText(u"account: %1\nroom id: %2\nroom name: %3\nupdated:\n%4"_s.arg(accountName, roomId, roomName, changes.join(u'\n')));
    });
    QObject::connect(job, &RocketChatRestApi::SaveRoomSettingsJob::failed, call, [call](const QString &serverErrorStr, const QString &descriptionError) {
        call->finishWithText(i18n("Update room settings failed: %1", descriptionError.isEmpty() ? serverErrorStr : descriptionError), true);
    });
    if (!job->start()) {
        call->finishWithText(i18n("Impossible to start update room settings job"), true);
    }
}
