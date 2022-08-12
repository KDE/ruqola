/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "restapimethodtest.h"
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
    QString newUrl = QStringLiteral("http://www.kde.org");
    rest.setServerUrl(newUrl);
    QCOMPARE(rest.serverUrl(), newUrl);
}

void RestApiMethodTest::shouldGenerateUrl()
{
    RestApiMethod rest;
    // If server url is empty return null url
    QVERIFY(!rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsList).isValid());
    rest.setServerUrl(QStringLiteral("http://www.kde.org"));
    QVERIFY(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsList).isValid());

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::Login), QUrl(QStringLiteral("http://www.kde.org/api/v1/login")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::Logout), QUrl(QStringLiteral("http://www.kde.org/api/v1/logout")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::Me), QUrl(QStringLiteral("http://www.kde.org/api/v1/me")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersCreate), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.create")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersCreateToken), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.createToken")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersGetPresence), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.getPresence")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersInfo), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.info")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersList), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.list")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersDelete), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.delete")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersRegister), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.register")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersGetAvatar), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.getAvatar")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersResetAvatar), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.resetAvatar")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersSetAvatar), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.setAvatar")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersUpdate), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.update")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersGetUsernameSuggestion),
             QUrl(QStringLiteral("http://www.kde.org/api/v1/users.getUsernameSuggestion")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersRemovePersonalAccessToken),
             QUrl(QStringLiteral("http://www.kde.org/api/v1/users.removePersonalAccessToken")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersGeneratePersonalAccessToken),
             QUrl(QStringLiteral("http://www.kde.org/api/v1/users.generatePersonalAccessToken")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersUpdateOwnBasicInfo),
             QUrl(QStringLiteral("http://www.kde.org/api/v1/users.updateOwnBasicInfo")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersRegeneratePersonalAccessToken),
             QUrl(QStringLiteral("http://www.kde.org/api/v1/users.regeneratePersonalAccessToken")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersSetStatus), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.setStatus")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersGetStatus), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.getStatus")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersPresence), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.presence")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersAutocomplete), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.autocomplete")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersRemoveOtherTokens), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.removeOtherTokens")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersSetActiveStatus), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.setActiveStatus")));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersResetTOTP), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.resetTOTP")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersResetE2EKey), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.resetE2EKey")));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatDelete), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.delete")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatGetMessage), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.getMessage")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatPinMessage), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.pinMessage")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatPostMessage), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.postMessage")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatReact), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.react")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatIgnoreUser), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.ignoreUser")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatStarMessage), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.starMessage")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatUnPinMessage), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.unPinMessage")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatUnStarMessage), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.unStarMessage")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatUpdate), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.update")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatSearch), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.search")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatReportMessage), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.reportMessage")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatFollowMessage), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.followMessage")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatUnFollowMessage), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.unfollowMessage")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatGetThreadsList), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.getThreadsList")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatSyncThreadMessages), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.syncThreadMessages")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatGetThreadMessages), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.getThreadMessages")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatSyncThreadsList), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.syncThreadsList")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatGetDeletedMessage), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.getDeletedMessages")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatSendMessage), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.sendMessage")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatMessageReadReceipts),
             QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.getMessageReadReceipts")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatGetPinnedMessages), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.getPinnedMessages")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatGetMentionedMessages),
             QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.getMentionedMessages")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatGetStarredMessages), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.getStarredMessages")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatGetSnippetedMessages),
             QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.getSnippetedMessages")));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsAddAll), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.addAll")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsAddLeader), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.addLeader")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsRemoveLeader), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.removeLeader")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsAddModerator), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.addModerator")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsAddOwner), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.addOwner")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsArchive), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.archive")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsModerators), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.moderators")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsClose), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.close")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsCreate), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.create")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsGetIntegrations),
             QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.getIntegrations")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsHistory), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.history")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsInfo), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.info")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsInvite), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.invite")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsKick), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.kick")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsLeave), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.leave")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsList), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.list")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsListJoined), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.list.joined")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsOpen), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.open")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsJoin), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.join")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsRemoveModerator),
             QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.removeModerator")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsRemoveOwner), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.removeOwner")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsRename), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.rename")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetDescription), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.setDescription")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetJoinCode), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.setJoinCode")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsRoles), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.roles")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetPurpose), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.setPurpose")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetReadOnly), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.setReadOnly")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetTopic), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.setTopic")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetType), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.setType")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsUnarchive), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.unarchive")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetAnnouncement),
             QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.setAnnouncement")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsFiles), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.files")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsMembers), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.members")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsCounters), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.counters")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsDelete), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.delete")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsGetAllUserMentionsByChannel),
             QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.getAllUserMentionsByChannel")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsOnline), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.online")));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsAddAll), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.addAll")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsAddModerator), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.addModerator")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsAddOwner), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.addOwner")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsArchive), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.archive")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsClose), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.close")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsCreate), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.create")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsGetIntegrations), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.getIntegrations")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsHistory), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.history")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsInfo), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.info")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsInvite), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.invite")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsKick), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.kick")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsLeave), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.leave")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsList), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.list")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsOpen), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.open")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsRemoveModerator), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.removeModerator")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupRemoveOwner), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.removeOwner")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsRename), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.rename")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsSetDescription), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.setDescription")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsSetPurpose), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.setPurpose")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsSetReadOnly), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.setReadOnly")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsSetTopic), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.setTopic")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsSetType), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.setType")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsListAll), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.listAll")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsMembers), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.members")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsUnarchive), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.unarchive")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsSetAnnouncement), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.setAnnouncement")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsRoles), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.roles")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsCounters), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.counters")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupRemoveLeader), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.removeLeader")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsAddLeader), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.addLeader")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsDelete), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.delete")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsSetEncrypted), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.setEncrypted")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ServerInfo), QUrl(QStringLiteral("http://www.kde.org/api/v1/info")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::Settings), QUrl(QStringLiteral("http://www.kde.org/api/v1/settings")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UpdateAdminSettings), QUrl(QStringLiteral("http://www.kde.org/api/v1/settings")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::SettingsPublic), QUrl(QStringLiteral("http://www.kde.org/api/v1/settings.public")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsUpload), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.upload")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::Spotlight), QUrl(QStringLiteral("http://www.kde.org/api/v1/spotlight")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ImClose), QUrl(QStringLiteral("http://www.kde.org/api/v1/im.close")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ImCreate), QUrl(QStringLiteral("http://www.kde.org/api/v1/im.create")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ImHistory), QUrl(QStringLiteral("http://www.kde.org/api/v1/im.history")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ImMessages), QUrl(QStringLiteral("http://www.kde.org/api/v1/im.messages")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ImMembers), QUrl(QStringLiteral("http://www.kde.org/api/v1/im.members")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ImOpen), QUrl(QStringLiteral("http://www.kde.org/api/v1/im.open")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ImSetTopic), QUrl(QStringLiteral("http://www.kde.org/api/v1/im.setTopic")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ImFiles), QUrl(QStringLiteral("http://www.kde.org/api/v1/im.files")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ImDelete), QUrl(QStringLiteral("http://www.kde.org/api/v1/im.delete")));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::LoadEmojiCustom), QUrl(QStringLiteral("http://www.kde.org/api/v1/emoji-custom.list")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::EmojiCustomDelete), QUrl(QStringLiteral("http://www.kde.org/api/v1/emoji-custom.delete")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::EmojiCustomCreate), QUrl(QStringLiteral("http://www.kde.org/api/v1/emoji-custom.create")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::EmojiCustomUpdate), QUrl(QStringLiteral("http://www.kde.org/api/v1/emoji-custom.update")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::EmojiCustomAll), QUrl(QStringLiteral("http://www.kde.org/api/v1/emoji-custom.all")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsSaveNotification), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.saveNotification")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsSaveSettings), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.saveRoomSettings")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsAdminRooms), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.adminRooms")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsAdminRoomsGetRoom), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.adminRooms.getRoom")));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::SubscriptionsRead), QUrl(QStringLiteral("http://www.kde.org/api/v1/subscriptions.read")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::SubscriptionsUnRead), QUrl(QStringLiteral("http://www.kde.org/api/v1/subscriptions.unread")));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsGet), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.get")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsFavorite), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.favorite")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsCleanHistory), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.cleanHistory")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsInfo), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.info")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsLeave), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.leave")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsCreateDiscussion), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.createDiscussion")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsGetDiscussions), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.getDiscussions")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsAutocompleteChannelAndPrivate),
             QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.autocomplete.channelAndPrivate")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsExport), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.export")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsChangeArchivactionState),
             QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.changeArchivationState")));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ForgotPassword), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.forgotPassword")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::CommandsList), QUrl(QStringLiteral("http://www.kde.org/api/v1/commands.list")));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::CommandsGet), QUrl(QStringLiteral("http://www.kde.org/api/v1/commands.get")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::CommandsRun), QUrl(QStringLiteral("http://www.kde.org/api/v1/commands.run")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::CommandsPreview), QUrl(QStringLiteral("http://www.kde.org/api/v1/commands.preview")));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::E2EfetchMyKeys), QUrl(QStringLiteral("http://www.kde.org/api/v1/e2e.fetchMyKeys")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::E2EupdateGroupKey), QUrl(QStringLiteral("http://www.kde.org/api/v1/e2e.updateGroupKey")));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::E2ESetRoomKeyID), QUrl(QStringLiteral("http://www.kde.org/api/v1/e2e.setRoomKeyID")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::E2ESetUserPublicAndPrivateKeys),
             QUrl(QStringLiteral("http://www.kde.org/api/v1/e2e.setUserPublicAndPrivateKeys")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::E2ERequestSubscriptionKeys),
             QUrl(QStringLiteral("http://www.kde.org/api/v1/e2e.requestSubscriptionKeys")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::E2EResetOwnE2EKey), QUrl(QStringLiteral("http://www.kde.org/api/v1/e2e.resetOwnE2EKey")));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RolesList), QUrl(QStringLiteral("http://www.kde.org/api/v1/roles.list")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RolesSync), QUrl(QStringLiteral("http://www.kde.org/api/v1/roles.sync")));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RolesCreate), QUrl(QStringLiteral("http://www.kde.org/api/v1/roles.create")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RolesUpdate), QUrl(QStringLiteral("http://www.kde.org/api/v1/roles.update")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RolesDelete), QUrl(QStringLiteral("http://www.kde.org/api/v1/roles.delete")));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RolesAddUserToRole), QUrl(QStringLiteral("http://www.kde.org/api/v1/roles.addUserToRole")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RolesGetUsersInRole), QUrl(QStringLiteral("http://www.kde.org/api/v1/roles.getUsersInRole")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RolesRemoveUserFromRole),
             QUrl(QStringLiteral("http://www.kde.org/api/v1/roles.removeUserFromRole")));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::VideoConfJitsi),
             QUrl(QStringLiteral("http://www.kde.org/api/v1/video-conference/jitsi.update-timeout")));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::AutoTranslateGetSupportedLanguages),
             QUrl(QStringLiteral("http://www.kde.org/api/v1/autotranslate.getSupportedLanguages")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::AutoTranslateSaveSettings),
             QUrl(QStringLiteral("http://www.kde.org/api/v1/autotranslate.saveSettings")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::AutoTranslateTranslateMessage),
             QUrl(QStringLiteral("http://www.kde.org/api/v1/autotranslate.translateMessage")));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::CustomUserStatusList), QUrl(QStringLiteral("http://www.kde.org/api/v1/custom-user-status.list")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::CustomUserStatusCreate),
             QUrl(QStringLiteral("http://www.kde.org/api/v1/custom-user-status.create")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::CustomUserStatusDelete),
             QUrl(QStringLiteral("http://www.kde.org/api/v1/custom-user-status.delete")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::CustomUserStatusUpdate),
             QUrl(QStringLiteral("http://www.kde.org/api/v1/custom-user-status.update")));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::CustomSoundsList), QUrl(QStringLiteral("http://www.kde.org/api/v1/custom-sounds.list")));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::FindOrCreateInvite), QUrl(QStringLiteral("http://www.kde.org/api/v1/findOrCreateInvite")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ListInvites), QUrl(QStringLiteral("http://www.kde.org/api/v1/listInvites")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RemoveInvite), QUrl(QStringLiteral("http://www.kde.org/api/v1/removeInvite")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UseInviteToken), QUrl(QStringLiteral("http://www.kde.org/api/v1/useInviteToken")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ValidateInviteToken), QUrl(QStringLiteral("http://www.kde.org/api/v1/validateInviteToken")));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::Users2FASendEmailCode), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.2fa.sendEmailCode")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::Users2FAEnableEmail), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.2fa.enableEmail")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::Users2FADisableEmail), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.2fa.disableEmail")));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersDeleteOwnAccount), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.deleteOwnAccount")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersSetPreferences), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.setPreferences")));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::PermissionsListAll), QUrl(QStringLiteral("http://www.kde.org/api/v1/permissions.listAll")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::PermissionsUpdate), QUrl(QStringLiteral("http://www.kde.org/api/v1/permissions.update")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::Statistics), QUrl(QStringLiteral("http://www.kde.org/api/v1/statistics")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::Directory), QUrl(QStringLiteral("http://www.kde.org/api/v1/directory")));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsList), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.list")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsListAll), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.listAll")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsCreate), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.create")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsAddRooms), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.addRooms")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsRemoveRoom), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.removeRoom")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsListRooms), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.listRooms")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsUpdateRoom), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.updateRoom")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsListRoomsOfUser), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.listRoomsOfUser")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsMembers), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.members")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsAddMembers), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.addMembers")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsUpdateMember), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.updateMember")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsRemoveMember), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.removeMember")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsLeave), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.leave")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsInfo), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.info")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsDelete), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.delete")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsAutocomplete), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.autocomplete")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsAutocompleteAvailableForTeams),
             QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.autocomplete.availableForTeams")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsConvertToTeam), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.convertToTeam")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsConvertToTeam), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.convertToTeam")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::StdoutQueue), QUrl(QStringLiteral("http://www.kde.org/api/v1/stdout.queue")));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::TeamsConvertToChannel), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.convertToChannel")));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::OauthAppsList), QUrl(QStringLiteral("http://www.kde.org/api/v1/oauth-apps.list")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::OauthAppsGet), QUrl(QStringLiteral("http://www.kde.org/api/v1/oauth-apps.get")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::SettingsOauth), QUrl(QStringLiteral("http://www.kde.org/api/v1/settings.oauth")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::SettingsAddCustomOauth), QUrl(QStringLiteral("http://www.kde.org/api/v1/settings.addCustomOAuth")));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::LicensesGet), QUrl(QStringLiteral("http://www.kde.org/api/v1/licenses.get")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::LicensesMaxActiveUsers), QUrl(QStringLiteral("http://www.kde.org/api/v1/licenses.maxActiveUsers")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::LicensesAdd), QUrl(QStringLiteral("http://www.kde.org/api/v1/licenses.add")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::LicensesIsEntreprise), QUrl(QStringLiteral("http://www.kde.org/api/v1/licenses.isEnterprise")));

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::BannersDismiss), QUrl(QStringLiteral("http://www.kde.org/api/v1/banners.dismiss")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::Banners), QUrl(QStringLiteral("http://www.kde.org/api/v1/banners")));
}
