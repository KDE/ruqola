/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "restapimethodtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include <QTest>

QTEST_GUILESS_MAIN(RestApiMethodTest)
using namespace RocketChatRestApi;
RestApiMethodTest::RestApiMethodTest(QObject *parent)
    : QObject(parent)
{
}

void RestApiMethodTest::shouldHaveDefaultValue()
{
    RestApiMethod rest;
    QVERIFY(rest.serverUrl().isEmpty());
}

void RestApiMethodTest::shouldAssignDefaultValue()
{
    RestApiMethod rest;
    QString newUrl = u"http://www.kde.org"_s;
    rest.setServerUrl(newUrl);
    QCOMPARE(rest.serverUrl(), newUrl);
}

void RestApiMethodTest::shouldGenerateUrl()
{
    RestApiMethod rest;
    // If server url is empty return null url
    QVERIFY(!rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsList).isValid());
    rest.setServerUrl(u"http://www.kde.org"_s);
    QVERIFY(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsList).isValid());

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::Login), QUrl(u"http://www.kde.org/api/v1/login"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::Logout), QUrl(u"http://www.kde.org/api/v1/logout"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::Me), QUrl(u"http://www.kde.org/api/v1/me"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersCreate), QUrl(u"http://www.kde.org/api/v1/users.create"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersCreateToken), QUrl(u"http://www.kde.org/api/v1/users.createToken"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersGetPresence), QUrl(u"http://www.kde.org/api/v1/users.getPresence"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersInfo), QUrl(u"http://www.kde.org/api/v1/users.info"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersList), QUrl(u"http://www.kde.org/api/v1/users.list"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersDelete), QUrl(u"http://www.kde.org/api/v1/users.delete"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersRegister), QUrl(u"http://www.kde.org/api/v1/users.register"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersGetAvatar), QUrl(u"http://www.kde.org/api/v1/users.getAvatar"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersResetAvatar), QUrl(u"http://www.kde.org/api/v1/users.resetAvatar"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersSetAvatar), QUrl(u"http://www.kde.org/api/v1/users.setAvatar"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersUpdate), QUrl(u"http://www.kde.org/api/v1/users.update"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersUpdateOwnBasicInfo), QUrl(u"http://www.kde.org/api/v1/users.updateOwnBasicInfo"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersGetUsernameSuggestion), QUrl(u"http://www.kde.org/api/v1/users.getUsernameSuggestion"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersRemovePersonalAccessToken),
             QUrl(u"http://www.kde.org/api/v1/users.removePersonalAccessToken"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersGeneratePersonalAccessToken),
             QUrl(u"http://www.kde.org/api/v1/users.generatePersonalAccessToken"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersRegeneratePersonalAccessToken),
             QUrl(u"http://www.kde.org/api/v1/users.regeneratePersonalAccessToken"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersGetPersonalAccessTokens), QUrl(u"http://www.kde.org/api/v1/users.getPersonalAccessTokens"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersSetStatus), QUrl(u"http://www.kde.org/api/v1/users.setStatus"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersGetStatus), QUrl(u"http://www.kde.org/api/v1/users.getStatus"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersPresence), QUrl(u"http://www.kde.org/api/v1/users.presence"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersAutocomplete), QUrl(u"http://www.kde.org/api/v1/users.autocomplete"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersRemoveOtherTokens), QUrl(u"http://www.kde.org/api/v1/users.removeOtherTokens"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersSetActiveStatus), QUrl(u"http://www.kde.org/api/v1/users.setActiveStatus"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersResetTOTP), QUrl(u"http://www.kde.org/api/v1/users.resetTOTP"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersResetE2EKey), QUrl(u"http://www.kde.org/api/v1/users.resetE2EKey"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatDelete), QUrl(u"http://www.kde.org/api/v1/chat.delete"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatGetMessage), QUrl(u"http://www.kde.org/api/v1/chat.getMessage"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatPinMessage), QUrl(u"http://www.kde.org/api/v1/chat.pinMessage"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatPostMessage), QUrl(u"http://www.kde.org/api/v1/chat.postMessage"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatReact), QUrl(u"http://www.kde.org/api/v1/chat.react"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatIgnoreUser), QUrl(u"http://www.kde.org/api/v1/chat.ignoreUser"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatStarMessage), QUrl(u"http://www.kde.org/api/v1/chat.starMessage"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatUnPinMessage), QUrl(u"http://www.kde.org/api/v1/chat.unPinMessage"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatUnStarMessage), QUrl(u"http://www.kde.org/api/v1/chat.unStarMessage"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatUpdate), QUrl(u"http://www.kde.org/api/v1/chat.update"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatSearch), QUrl(u"http://www.kde.org/api/v1/chat.search"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatReportMessage), QUrl(u"http://www.kde.org/api/v1/chat.reportMessage"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatFollowMessage), QUrl(u"http://www.kde.org/api/v1/chat.followMessage"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatUnFollowMessage), QUrl(u"http://www.kde.org/api/v1/chat.unfollowMessage"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatGetThreadsList), QUrl(u"http://www.kde.org/api/v1/chat.getThreadsList"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatSyncThreadMessages), QUrl(u"http://www.kde.org/api/v1/chat.syncThreadMessages"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatGetThreadMessages), QUrl(u"http://www.kde.org/api/v1/chat.getThreadMessages"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatSyncThreadsList), QUrl(u"http://www.kde.org/api/v1/chat.syncThreadsList"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatGetDeletedMessage), QUrl(u"http://www.kde.org/api/v1/chat.getDeletedMessages"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatSendMessage), QUrl(u"http://www.kde.org/api/v1/chat.sendMessage"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatMessageReadReceipts), QUrl(u"http://www.kde.org/api/v1/chat.getMessageReadReceipts"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatGetPinnedMessages), QUrl(u"http://www.kde.org/api/v1/chat.getPinnedMessages"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatGetMentionedMessages), QUrl(u"http://www.kde.org/api/v1/chat.getMentionedMessages"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatGetStarredMessages), QUrl(u"http://www.kde.org/api/v1/chat.getStarredMessages"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatGetSnippetedMessages), QUrl(u"http://www.kde.org/api/v1/chat.getSnippetedMessages"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatSyncMessages), QUrl(u"http://www.kde.org/api/v1/chat.syncMessages"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsAddAll), QUrl(u"http://www.kde.org/api/v1/channels.addAll"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsAddLeader), QUrl(u"http://www.kde.org/api/v1/channels.addLeader"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsRemoveLeader), QUrl(u"http://www.kde.org/api/v1/channels.removeLeader"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsAddModerator), QUrl(u"http://www.kde.org/api/v1/channels.addModerator"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsAddOwner), QUrl(u"http://www.kde.org/api/v1/channels.addOwner"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsArchive), QUrl(u"http://www.kde.org/api/v1/channels.archive"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsModerators), QUrl(u"http://www.kde.org/api/v1/channels.moderators"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsClose), QUrl(u"http://www.kde.org/api/v1/channels.close"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsCreate), QUrl(u"http://www.kde.org/api/v1/channels.create"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsGetIntegrations), QUrl(u"http://www.kde.org/api/v1/channels.getIntegrations"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsHistory), QUrl(u"http://www.kde.org/api/v1/channels.history"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsInfo), QUrl(u"http://www.kde.org/api/v1/channels.info"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsInvite), QUrl(u"http://www.kde.org/api/v1/channels.invite"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsKick), QUrl(u"http://www.kde.org/api/v1/channels.kick"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsLeave), QUrl(u"http://www.kde.org/api/v1/channels.leave"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsList), QUrl(u"http://www.kde.org/api/v1/channels.list"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsListJoined), QUrl(u"http://www.kde.org/api/v1/channels.list.joined"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsOpen), QUrl(u"http://www.kde.org/api/v1/channels.open"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsJoin), QUrl(u"http://www.kde.org/api/v1/channels.join"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsRemoveModerator), QUrl(u"http://www.kde.org/api/v1/channels.removeModerator"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsRemoveOwner), QUrl(u"http://www.kde.org/api/v1/channels.removeOwner"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsRename), QUrl(u"http://www.kde.org/api/v1/channels.rename"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetDescription), QUrl(u"http://www.kde.org/api/v1/channels.setDescription"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetJoinCode), QUrl(u"http://www.kde.org/api/v1/channels.setJoinCode"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsRoles), QUrl(u"http://www.kde.org/api/v1/channels.roles"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetPurpose), QUrl(u"http://www.kde.org/api/v1/channels.setPurpose"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetReadOnly), QUrl(u"http://www.kde.org/api/v1/channels.setReadOnly"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetTopic), QUrl(u"http://www.kde.org/api/v1/channels.setTopic"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetType), QUrl(u"http://www.kde.org/api/v1/channels.setType"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsUnarchive), QUrl(u"http://www.kde.org/api/v1/channels.unarchive"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetAnnouncement), QUrl(u"http://www.kde.org/api/v1/channels.setAnnouncement"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsFiles), QUrl(u"http://www.kde.org/api/v1/channels.files"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsMembers), QUrl(u"http://www.kde.org/api/v1/channels.members"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsCounters), QUrl(u"http://www.kde.org/api/v1/channels.counters"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsDelete), QUrl(u"http://www.kde.org/api/v1/channels.delete"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsGetAllUserMentionsByChannel),
             QUrl(u"http://www.kde.org/api/v1/channels.getAllUserMentionsByChannel"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsOnline), QUrl(u"http://www.kde.org/api/v1/channels.online"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsAddAll), QUrl(u"http://www.kde.org/api/v1/groups.addAll"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsAddModerator), QUrl(u"http://www.kde.org/api/v1/groups.addModerator"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsAddOwner), QUrl(u"http://www.kde.org/api/v1/groups.addOwner"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsArchive), QUrl(u"http://www.kde.org/api/v1/groups.archive"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsClose), QUrl(u"http://www.kde.org/api/v1/groups.close"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsCreate), QUrl(u"http://www.kde.org/api/v1/groups.create"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsGetIntegrations), QUrl(u"http://www.kde.org/api/v1/groups.getIntegrations"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsHistory), QUrl(u"http://www.kde.org/api/v1/groups.history"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsInfo), QUrl(u"http://www.kde.org/api/v1/groups.info"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsInvite), QUrl(u"http://www.kde.org/api/v1/groups.invite"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsKick), QUrl(u"http://www.kde.org/api/v1/groups.kick"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsLeave), QUrl(u"http://www.kde.org/api/v1/groups.leave"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsList), QUrl(u"http://www.kde.org/api/v1/groups.list"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsOpen), QUrl(u"http://www.kde.org/api/v1/groups.open"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsRemoveModerator), QUrl(u"http://www.kde.org/api/v1/groups.removeModerator"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsRemoveOwner), QUrl(u"http://www.kde.org/api/v1/groups.removeOwner"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsRename), QUrl(u"http://www.kde.org/api/v1/groups.rename"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsSetDescription), QUrl(u"http://www.kde.org/api/v1/groups.setDescription"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsSetPurpose), QUrl(u"http://www.kde.org/api/v1/groups.setPurpose"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsSetReadOnly), QUrl(u"http://www.kde.org/api/v1/groups.setReadOnly"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsSetTopic), QUrl(u"http://www.kde.org/api/v1/groups.setTopic"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsSetType), QUrl(u"http://www.kde.org/api/v1/groups.setType"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsListAll), QUrl(u"http://www.kde.org/api/v1/groups.listAll"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsMembers), QUrl(u"http://www.kde.org/api/v1/groups.members"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsUnarchive), QUrl(u"http://www.kde.org/api/v1/groups.unarchive"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsSetAnnouncement), QUrl(u"http://www.kde.org/api/v1/groups.setAnnouncement"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsRoles), QUrl(u"http://www.kde.org/api/v1/groups.roles"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsCounters), QUrl(u"http://www.kde.org/api/v1/groups.counters"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsRemoveLeader), QUrl(u"http://www.kde.org/api/v1/groups.removeLeader"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsAddLeader), QUrl(u"http://www.kde.org/api/v1/groups.addLeader"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsDelete), QUrl(u"http://www.kde.org/api/v1/groups.delete"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsSetEncrypted), QUrl(u"http://www.kde.org/api/v1/groups.setEncrypted"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ServerInfo), QUrl(u"http://www.kde.org/api/v1/info"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::Settings), QUrl(u"http://www.kde.org/api/v1/settings"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UpdateAdminSettings), QUrl(u"http://www.kde.org/api/v1/settings"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::SettingsPublic), QUrl(u"http://www.kde.org/api/v1/settings.public"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsUpload), QUrl(u"http://www.kde.org/api/v1/rooms.upload"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::Spotlight), QUrl(u"http://www.kde.org/api/v1/spotlight"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ImClose), QUrl(u"http://www.kde.org/api/v1/im.close"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ImCreate), QUrl(u"http://www.kde.org/api/v1/im.create"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ImHistory), QUrl(u"http://www.kde.org/api/v1/im.history"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ImMessages), QUrl(u"http://www.kde.org/api/v1/im.messages"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ImMembers), QUrl(u"http://www.kde.org/api/v1/im.members"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ImOpen), QUrl(u"http://www.kde.org/api/v1/im.open"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ImSetTopic), QUrl(u"http://www.kde.org/api/v1/im.setTopic"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ImFiles), QUrl(u"http://www.kde.org/api/v1/im.files"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ImDelete), QUrl(u"http://www.kde.org/api/v1/im.delete"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::LoadEmojiCustom), QUrl(u"http://www.kde.org/api/v1/emoji-custom.list"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::EmojiCustomDelete), QUrl(u"http://www.kde.org/api/v1/emoji-custom.delete"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::EmojiCustomCreate), QUrl(u"http://www.kde.org/api/v1/emoji-custom.create"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::EmojiCustomUpdate), QUrl(u"http://www.kde.org/api/v1/emoji-custom.update"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::EmojiCustomAll), QUrl(u"http://www.kde.org/api/v1/emoji-custom.all"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsSaveNotification), QUrl(u"http://www.kde.org/api/v1/rooms.saveNotification"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsSaveSettings), QUrl(u"http://www.kde.org/api/v1/rooms.saveRoomSettings"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsAdminRooms), QUrl(u"http://www.kde.org/api/v1/rooms.adminRooms"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsAdminRoomsGetRoom), QUrl(u"http://www.kde.org/api/v1/rooms.adminRooms.getRoom"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::SubscriptionsRead), QUrl(u"http://www.kde.org/api/v1/subscriptions.read"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::SubscriptionsUnRead), QUrl(u"http://www.kde.org/api/v1/subscriptions.unread"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsGet), QUrl(u"http://www.kde.org/api/v1/rooms.get"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsFavorite), QUrl(u"http://www.kde.org/api/v1/rooms.favorite"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsCleanHistory), QUrl(u"http://www.kde.org/api/v1/rooms.cleanHistory"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsInfo), QUrl(u"http://www.kde.org/api/v1/rooms.info"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsLeave), QUrl(u"http://www.kde.org/api/v1/rooms.leave"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsCreateDiscussion), QUrl(u"http://www.kde.org/api/v1/rooms.createDiscussion"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsGetDiscussions), QUrl(u"http://www.kde.org/api/v1/rooms.getDiscussions"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsAutocompleteChannelAndPrivate),
             QUrl(u"http://www.kde.org/api/v1/rooms.autocomplete.channelAndPrivate"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsExport), QUrl(u"http://www.kde.org/api/v1/rooms.export"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsChangeArchivactionState), QUrl(u"http://www.kde.org/api/v1/rooms.changeArchivationState"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsDelete), QUrl(u"http://www.kde.org/api/v1/rooms.delete"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ForgotPassword), QUrl(u"http://www.kde.org/api/v1/users.forgotPassword"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::CommandsList), QUrl(u"http://www.kde.org/api/v1/commands.list"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::CommandsGet), QUrl(u"http://www.kde.org/api/v1/commands.get"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::CommandsRun), QUrl(u"http://www.kde.org/api/v1/commands.run"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::CommandsPreview), QUrl(u"http://www.kde.org/api/v1/commands.preview"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::E2EfetchMyKeys), QUrl(u"http://www.kde.org/api/v1/e2e.fetchMyKeys"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::E2EupdateGroupKey), QUrl(u"http://www.kde.org/api/v1/e2e.updateGroupKey"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::E2ESetRoomKeyID), QUrl(u"http://www.kde.org/api/v1/e2e.setRoomKeyID"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::E2ESetUserPublicAndPrivateKeys),
             QUrl(u"http://www.kde.org/api/v1/e2e.setUserPublicAndPrivateKeys"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::E2ERequestSubscriptionKeys), QUrl(u"http://www.kde.org/api/v1/e2e.requestSubscriptionKeys"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::E2EResetOwnE2EKey), QUrl(u"http://www.kde.org/api/v1/e2e.resetOwnE2EKey"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::E2EAcceptSuggestedGroupKey), QUrl(u"http://www.kde.org/api/v1/e2e.acceptSuggestedGroupKey"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::E2ERejectSuggestedGroupKey), QUrl(u"http://www.kde.org/api/v1/e2e.rejectSuggestedGroupKey"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::E2EProvideUsersWithSuggestedGroupKeys),
             QUrl(u"http://www.kde.org/api/v1/e2e.provideUsersWithSuggestedGroupKeys"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::E2EResetRoomKey), QUrl(u"http://www.kde.org/api/v1/e2e.resetRoomKey"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::E2EFetchUsersWaitingForGroupKey),
             QUrl(u"http://www.kde.org/api/v1/e2e.provideUsersSuggestedGroupKeys"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RolesList), QUrl(u"http://www.kde.org/api/v1/roles.list"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RolesSync), QUrl(u"http://www.kde.org/api/v1/roles.sync"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RolesCreate), QUrl(u"http://www.kde.org/api/v1/roles.create"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RolesUpdate), QUrl(u"http://www.kde.org/api/v1/roles.update"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RolesDelete), QUrl(u"http://www.kde.org/api/v1/roles.delete"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RolesAddUserToRole), QUrl(u"http://www.kde.org/api/v1/roles.addUserToRole"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RolesGetUsersInRole), QUrl(u"http://www.kde.org/api/v1/roles.getUsersInRole"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RolesRemoveUserFromRole), QUrl(u"http://www.kde.org/api/v1/roles.removeUserFromRole"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::VideoConfJitsi), QUrl(u"http://www.kde.org/api/v1/video-conference/jitsi.update-timeout"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::AutoTranslateGetSupportedLanguages),
             QUrl(u"http://www.kde.org/api/v1/autotranslate.getSupportedLanguages"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::AutoTranslateSaveSettings), QUrl(u"http://www.kde.org/api/v1/autotranslate.saveSettings"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::AutoTranslateTranslateMessage), QUrl(u"http://www.kde.org/api/v1/autotranslate.translateMessage"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::CustomUserStatusList), QUrl(u"http://www.kde.org/api/v1/custom-user-status.list"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::CustomUserStatusCreate), QUrl(u"http://www.kde.org/api/v1/custom-user-status.create"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::CustomUserStatusDelete), QUrl(u"http://www.kde.org/api/v1/custom-user-status.delete"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::CustomUserStatusUpdate), QUrl(u"http://www.kde.org/api/v1/custom-user-status.update"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::CustomSoundsList), QUrl(u"http://www.kde.org/api/v1/custom-sounds.list"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::FindOrCreateInvite), QUrl(u"http://www.kde.org/api/v1/findOrCreateInvite"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ListInvites), QUrl(u"http://www.kde.org/api/v1/listInvites"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RemoveInvite), QUrl(u"http://www.kde.org/api/v1/removeInvite"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UseInviteToken), QUrl(u"http://www.kde.org/api/v1/useInviteToken"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ValidateInviteToken), QUrl(u"http://www.kde.org/api/v1/validateInviteToken"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::SendInvitationEmails), QUrl(u"http://www.kde.org/api/v1/sendInvitationEmail"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::Users2FASendEmailCode), QUrl(u"http://www.kde.org/api/v1/users.2fa.sendEmailCode"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::Users2FAEnableEmail), QUrl(u"http://www.kde.org/api/v1/users.2fa.enableEmail"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::Users2FADisableEmail), QUrl(u"http://www.kde.org/api/v1/users.2fa.disableEmail"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersDeleteOwnAccount), QUrl(u"http://www.kde.org/api/v1/users.deleteOwnAccount"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersSetPreferences), QUrl(u"http://www.kde.org/api/v1/users.setPreferences"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::PermissionsListAll), QUrl(u"http://www.kde.org/api/v1/permissions.listAll"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::PermissionsUpdate), QUrl(u"http://www.kde.org/api/v1/permissions.update"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::Statistics), QUrl(u"http://www.kde.org/api/v1/statistics"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::Directory), QUrl(u"http://www.kde.org/api/v1/directory"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsList), QUrl(u"http://www.kde.org/api/v1/teams.list"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsListAll), QUrl(u"http://www.kde.org/api/v1/teams.listAll"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsCreate), QUrl(u"http://www.kde.org/api/v1/teams.create"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsAddRooms), QUrl(u"http://www.kde.org/api/v1/teams.addRooms"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsRemoveRoom), QUrl(u"http://www.kde.org/api/v1/teams.removeRoom"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsListRooms), QUrl(u"http://www.kde.org/api/v1/teams.listRooms"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsUpdateRoom), QUrl(u"http://www.kde.org/api/v1/teams.updateRoom"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsListRoomsOfUser), QUrl(u"http://www.kde.org/api/v1/teams.listRoomsOfUser"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsMembers), QUrl(u"http://www.kde.org/api/v1/teams.members"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsAddMembers), QUrl(u"http://www.kde.org/api/v1/teams.addMembers"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsUpdateMember), QUrl(u"http://www.kde.org/api/v1/teams.updateMember"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsRemoveMember), QUrl(u"http://www.kde.org/api/v1/teams.removeMember"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsLeave), QUrl(u"http://www.kde.org/api/v1/teams.leave"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsInfo), QUrl(u"http://www.kde.org/api/v1/teams.info"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsDelete), QUrl(u"http://www.kde.org/api/v1/teams.delete"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsAutocomplete), QUrl(u"http://www.kde.org/api/v1/teams.autocomplete"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsAutocompleteAvailableForTeams),
             QUrl(u"http://www.kde.org/api/v1/rooms.autocomplete.availableForTeams"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsConvertToTeam), QUrl(u"http://www.kde.org/api/v1/channels.convertToTeam"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsConvertToTeam), QUrl(u"http://www.kde.org/api/v1/groups.convertToTeam"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::StdoutQueue), QUrl(u"http://www.kde.org/api/v1/stdout.queue"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsConvertToChannel), QUrl(u"http://www.kde.org/api/v1/teams.convertToChannel"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::OauthAppsList), QUrl(u"http://www.kde.org/api/v1/oauth-apps.list"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::OauthAppsGet), QUrl(u"http://www.kde.org/api/v1/oauth-apps.get"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::OauthAppsCreate), QUrl(u"http://www.kde.org/api/v1/oauth-apps.create"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::OauthAppsUpdate), QUrl(u"http://www.kde.org/api/v1/oauth-apps.update"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::OauthAppsDelete), QUrl(u"http://www.kde.org/api/v1/oauth-apps.delete"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::SettingsOauth), QUrl(u"http://www.kde.org/api/v1/settings.oauth"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::SettingsAddCustomOauth), QUrl(u"http://www.kde.org/api/v1/settings.addCustomOAuth"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::LicensesMaxActiveUsers), QUrl(u"http://www.kde.org/api/v1/licenses.maxActiveUsers"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::LicensesAdd), QUrl(u"http://www.kde.org/api/v1/licenses.add"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::LicensesIsEntreprise), QUrl(u"http://www.kde.org/api/v1/licenses.isEnterprise"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::LicensesInfo), QUrl(u"http://www.kde.org/api/v1/licenses.info"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::BannersDismiss), QUrl(u"http://www.kde.org/api/v1/banners.dismiss"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::Banners), QUrl(u"http://www.kde.org/api/v1/banners"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::SessionsList), QUrl(u"http://www.kde.org/api/v1/sessions/list"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::SessionsInfo), QUrl(u"http://www.kde.org/api/v1/sessions/info"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::SessionsListAll), QUrl(u"http://www.kde.org/api/v1/sessions/list.all"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::SessionsLogoutMe), QUrl(u"http://www.kde.org/api/v1/sessions/logout.me"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::SessionsLogout), QUrl(u"http://www.kde.org/api/v1/sessions/logout"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::SessionsInfoAdmin), QUrl(u"http://www.kde.org/api/v1/sessions/info.admin"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UserRequestDataDownload), QUrl(u"http://www.kde.org/api/v1/users.requestDataDownload"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::VideoConferenceStart), QUrl(u"http://www.kde.org/api/v1/video-conference.start"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::VideoConferenceJoin), QUrl(u"http://www.kde.org/api/v1/video-conference.join"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::VideoConferenceCancel), QUrl(u"http://www.kde.org/api/v1/video-conference.cancel"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::VideoConferenceInfo), QUrl(u"http://www.kde.org/api/v1/video-conference.info"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::VideoConferenceList), QUrl(u"http://www.kde.org/api/v1/video-conference.list"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::VideoConferenceProviders), QUrl(u"http://www.kde.org/api/v1/video-conference.providers"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::VideoConferenceCapabilities), QUrl(u"http://www.kde.org/api/v1/video-conference.capabilities"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ModerationReportsByUsers), QUrl(u"http://www.kde.org/api/v1/moderation.reportsByUsers"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ModerationDismissReports), QUrl(u"http://www.kde.org/api/v1/moderation.dismissReports"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ModerationUserReportedMessages),
             QUrl(u"http://www.kde.org/api/v1/moderation.user.reportedMessages"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ModerationUserDeleteReportedMessages),
             QUrl(u"http://www.kde.org/api/v1/moderation.user.deleteReportedMessages"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ModerationReports), QUrl(u"http://www.kde.org/api/v1/moderation.reports"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ModerationReportInfo), QUrl(u"http://www.kde.org/api/v1/moderation.reportInfo"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ModerationUserReports), QUrl(u"http://www.kde.org/api/v1/moderation.userReports"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ModerationUserReportsByUserId),
             QUrl(u"http://www.kde.org/api/v1/moderation.user.reportsByUserId"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ModerationDismissUserReports), QUrl(u"http://www.kde.org/api/v1/moderation.dismissUserReports"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ModerationReportUser), QUrl(u"http://www.kde.org/api/v1/moderation.reportUser"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsNameExists), QUrl(u"http://www.kde.org/api/v1/rooms.nameExists"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::MethodCall), QUrl(u"http://www.kde.org/api/v1/method.call"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::MethodCallAnon), QUrl(u"http://www.kde.org/api/v1/method.callAnon"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::AppsUiInteraction, RestApiUtil::RestApiUrlExtensionType::Apps),
             QUrl(u"http://www.kde.org/api/apps/ui.interaction"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersLogoutOtherClients), QUrl(u"http://www.kde.org/api/v1/users.logoutOtherClients"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsMuteUser), QUrl(u"http://www.kde.org/api/v1/rooms.muteUser"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsUnmuteUser), QUrl(u"http://www.kde.org/api/v1/rooms.unmuteUser"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::CategoriesApps, RestApiUtil::RestApiUrlExtensionType::Apps),
             QUrl(u"http://www.kde.org/api/apps/categories"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::FeaturedApps, RestApiUtil::RestApiUrlExtensionType::Apps),
             QUrl(u"http://www.kde.org/api/apps/featured-apps"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::CountApps, RestApiUtil::RestApiUrlExtensionType::Apps),
             QUrl(u"http://www.kde.org/api/apps/count"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::NotifyAdminsApps, RestApiUtil::RestApiUrlExtensionType::Apps),
             QUrl(u"http://www.kde.org/api/apps/notify-admins"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::InstalledApps, RestApiUtil::RestApiUrlExtensionType::Apps),
             QUrl(u"http://www.kde.org/api/apps/installed"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::MarketplaceApps, RestApiUtil::RestApiUrlExtensionType::Apps),
             QUrl(u"http://www.kde.org/api/apps/marketplace"_s));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersListByStatus), QUrl(u"http://www.kde.org/api/v1/users.listByStatus"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersSendWelcomeEmail), QUrl(u"http://www.kde.org/api/v1/users.sendWelcomeEmail"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::Empty), QUrl(u"http://www.kde.org/api/v1/"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsImages), QUrl(u"http://www.kde.org/api/v1/rooms.images"_s));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsMembersOrderedByRole), QUrl(u"http://www.kde.org/api/v1/rooms.membersOrderedByRole"_s));
}

#include "moc_restapimethodtest.cpp"
