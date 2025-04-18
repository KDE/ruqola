/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "restapiutil.h"
#include "rocketchatqtrestapi_debug.h"
#include <QCryptographicHash>
using namespace Qt::Literals::StringLiterals;
QString RestApiUtil::adaptUrl(const QString &url)
{
    if (url.isEmpty()) {
        return url;
    }
    // Avoid to add more https:// :)
    if (url.startsWith("https://"_L1) || url.startsWith("http://"_L1)) {
        return url;
    } else {
        // Default to https
        return QStringLiteral("https://") + url;
    }
}

QString RestApiUtil::restUrl(RestApiUtil::RestApiUrlType type)
{
    switch (type) {
    case RestApiUtil::RestApiUrlType::Login:
        return QStringLiteral("login");
    case RestApiUtil::RestApiUrlType::Logout:
        return QStringLiteral("logout");
    case RestApiUtil::RestApiUrlType::Me:
        return QStringLiteral("me");
    case RestApiUtil::RestApiUrlType::UsersGetAvatar:
        return QStringLiteral("users.getAvatar");
    case RestApiUtil::RestApiUrlType::UsersDelete:
        return QStringLiteral("users.delete");
    case RestApiUtil::RestApiUrlType::UsersCreate:
        return QStringLiteral("users.create");
    case RestApiUtil::RestApiUrlType::UsersCreateToken:
        return QStringLiteral("users.createToken");
    case RestApiUtil::RestApiUrlType::UsersGetPresence:
        return QStringLiteral("users.getPresence");
    case RestApiUtil::RestApiUrlType::UsersInfo:
        return QStringLiteral("users.info");
    case RestApiUtil::RestApiUrlType::UsersList:
        return QStringLiteral("users.list");
    case RestApiUtil::RestApiUrlType::UsersRegister:
        return QStringLiteral("users.register");
    case RestApiUtil::RestApiUrlType::UsersResetAvatar:
        return QStringLiteral("users.resetAvatar");
    case RestApiUtil::RestApiUrlType::UsersSetAvatar:
        return QStringLiteral("users.setAvatar");
    case RestApiUtil::RestApiUrlType::UsersUpdate:
        return QStringLiteral("users.update");
    case RestApiUtil::RestApiUrlType::UsersGetUsernameSuggestion:
        return QStringLiteral("users.getUsernameSuggestion");

    case RestApiUtil::RestApiUrlType::UsersRemovePersonalAccessToken:
        return QStringLiteral("users.removePersonalAccessToken");
    case RestApiUtil::RestApiUrlType::UsersGeneratePersonalAccessToken:
        return QStringLiteral("users.generatePersonalAccessToken");
    case RestApiUtil::RestApiUrlType::UsersRegeneratePersonalAccessToken:
        return QStringLiteral("users.regeneratePersonalAccessToken");
    case RestApiUtil::RestApiUrlType::UsersGetPersonalAccessTokens:
        return QStringLiteral("users.getPersonalAccessTokens");

    case RestApiUtil::RestApiUrlType::UsersPresence:
        return QStringLiteral("users.presence");
    case RestApiUtil::RestApiUrlType::UsersUpdateOwnBasicInfo:
        return QStringLiteral("users.updateOwnBasicInfo");
    case RestApiUtil::RestApiUrlType::UsersSetStatus:
        return QStringLiteral("users.setStatus");
    case RestApiUtil::RestApiUrlType::UsersGetStatus:
        return QStringLiteral("users.getStatus");
    case RestApiUtil::RestApiUrlType::UsersAutocomplete:
        return QStringLiteral("users.autocomplete");
    case RestApiUtil::RestApiUrlType::UsersRemoveOtherTokens:
        return QStringLiteral("users.removeOtherTokens");
    case RestApiUtil::RestApiUrlType::UsersSetActiveStatus:
        return QStringLiteral("users.setActiveStatus");
    case RestApiUtil::RestApiUrlType::UsersResetTOTP:
        return QStringLiteral("users.resetTOTP");
    case RestApiUtil::RestApiUrlType::UsersResetE2EKey:
        return QStringLiteral("users.resetE2EKey");

    case RestApiUtil::RestApiUrlType::ChatDelete:
        return QStringLiteral("chat.delete");
    case RestApiUtil::RestApiUrlType::ChatGetMessage:
        return QStringLiteral("chat.getMessage");
    case RestApiUtil::RestApiUrlType::ChatPinMessage:
        return QStringLiteral("chat.pinMessage");
    case RestApiUtil::RestApiUrlType::ChatPostMessage:
        return QStringLiteral("chat.postMessage");
    case RestApiUtil::RestApiUrlType::ChatReact:
        return QStringLiteral("chat.react");
    case RestApiUtil::RestApiUrlType::ChatStarMessage:
        return QStringLiteral("chat.starMessage");
    case RestApiUtil::RestApiUrlType::ChatUnPinMessage:
        return QStringLiteral("chat.unPinMessage");
    case RestApiUtil::RestApiUrlType::ChatUnStarMessage:
        return QStringLiteral("chat.unStarMessage");
    case RestApiUtil::RestApiUrlType::ChatUpdate:
        return QStringLiteral("chat.update");
    case RestApiUtil::RestApiUrlType::ChatSearch:
        return QStringLiteral("chat.search");
    case RestApiUtil::RestApiUrlType::ChatIgnoreUser:
        return QStringLiteral("chat.ignoreUser");
    case RestApiUtil::RestApiUrlType::ChatReportMessage:
        return QStringLiteral("chat.reportMessage");
    case RestApiUtil::RestApiUrlType::ChatFollowMessage:
        return QStringLiteral("chat.followMessage");
    case RestApiUtil::RestApiUrlType::ChatUnFollowMessage:
        return QStringLiteral("chat.unfollowMessage");
    case RestApiUtil::RestApiUrlType::ChatGetDeletedMessage:
        return QStringLiteral("chat.getDeletedMessages");
    case RestApiUtil::RestApiUrlType::ChatMessageReadReceipts:
        return QStringLiteral("chat.getMessageReadReceipts");
    case RestApiUtil::RestApiUrlType::ChatSyncThreadsList:
        return QStringLiteral("chat.syncThreadsList");
    case RestApiUtil::RestApiUrlType::ChatGetThreadsList:
        return QStringLiteral("chat.getThreadsList");
    case RestApiUtil::RestApiUrlType::ChatSyncThreadMessages:
        return QStringLiteral("chat.syncThreadMessages");
    case RestApiUtil::RestApiUrlType::ChatGetThreadMessages:
        return QStringLiteral("chat.getThreadMessages");
    case RestApiUtil::RestApiUrlType::ChatSendMessage:
        return QStringLiteral("chat.sendMessage");
    case RestApiUtil::RestApiUrlType::ChatGetPinnedMessages:
        return QStringLiteral("chat.getPinnedMessages");
    case RestApiUtil::RestApiUrlType::ChatGetMentionedMessages:
        return QStringLiteral("chat.getMentionedMessages");
    case RestApiUtil::RestApiUrlType::ChatGetStarredMessages:
        return QStringLiteral("chat.getStarredMessages");
    case RestApiUtil::RestApiUrlType::ChatGetSnippetedMessages:
        return QStringLiteral("chat.getSnippetedMessages");
    case RestApiUtil::RestApiUrlType::ChatSyncMessages:
        return QStringLiteral("chat.syncMessages");
    case RestApiUtil::RestApiUrlType::ChannelsAddAll:
        return QStringLiteral("channels.addAll");
    case RestApiUtil::RestApiUrlType::ChannelsAddModerator:
        return QStringLiteral("channels.addModerator");
    case RestApiUtil::RestApiUrlType::ChannelsAddOwner:
        return QStringLiteral("channels.addOwner");
    case RestApiUtil::RestApiUrlType::ChannelsAddLeader:
        return QStringLiteral("channels.addLeader");
    case RestApiUtil::RestApiUrlType::ChannelsArchive:
        return QStringLiteral("channels.archive");
    case RestApiUtil::RestApiUrlType::ChannelsModerators:
        return QStringLiteral("channels.moderators");
    case RestApiUtil::RestApiUrlType::ChannelsClose:
        return QStringLiteral("channels.close");
    case RestApiUtil::RestApiUrlType::ChannelsCreate:
        return QStringLiteral("channels.create");
    case RestApiUtil::RestApiUrlType::ChannelsGetIntegrations:
        return QStringLiteral("channels.getIntegrations");
    case RestApiUtil::RestApiUrlType::ChannelsHistory:
        return QStringLiteral("channels.history");
    case RestApiUtil::RestApiUrlType::ChannelsInfo:
        return QStringLiteral("channels.info");
    case RestApiUtil::RestApiUrlType::ChannelsInvite:
        return QStringLiteral("channels.invite");
    case RestApiUtil::RestApiUrlType::ChannelsKick:
        return QStringLiteral("channels.kick");
    case RestApiUtil::RestApiUrlType::ChannelsLeave:
        return QStringLiteral("channels.leave");
    case RestApiUtil::RestApiUrlType::ChannelsList:
        return QStringLiteral("channels.list");
    case RestApiUtil::RestApiUrlType::ChannelsListJoined:
        return QStringLiteral("channels.list.joined");
    case RestApiUtil::RestApiUrlType::ChannelsOpen:
        return QStringLiteral("channels.open");
    case RestApiUtil::RestApiUrlType::ChannelsRemoveModerator:
        return QStringLiteral("channels.removeModerator");
    case RestApiUtil::RestApiUrlType::ChannelsRemoveOwner:
        return QStringLiteral("channels.removeOwner");
    case RestApiUtil::RestApiUrlType::ChannelsRemoveLeader:
        return QStringLiteral("channels.removeLeader");
    case RestApiUtil::RestApiUrlType::ChannelsRename:
        return QStringLiteral("channels.rename");
    case RestApiUtil::RestApiUrlType::ChannelsSetDescription:
        return QStringLiteral("channels.setDescription");
    case RestApiUtil::RestApiUrlType::ChannelsSetJoinCode:
        return QStringLiteral("channels.setJoinCode");
    case RestApiUtil::RestApiUrlType::ChannelsSetPurpose:
        return QStringLiteral("channels.setPurpose");
    case RestApiUtil::RestApiUrlType::ChannelsSetReadOnly:
        return QStringLiteral("channels.setReadOnly");
    case RestApiUtil::RestApiUrlType::ChannelsSetTopic:
        return QStringLiteral("channels.setTopic");
    case RestApiUtil::RestApiUrlType::ChannelsSetAnnouncement:
        return QStringLiteral("channels.setAnnouncement");
    case RestApiUtil::RestApiUrlType::ChannelsFiles:
        return QStringLiteral("channels.files");
    case RestApiUtil::RestApiUrlType::ChannelsSetType:
        return QStringLiteral("channels.setType");
    case RestApiUtil::RestApiUrlType::ChannelsUnarchive:
        return QStringLiteral("channels.unarchive");
    case RestApiUtil::RestApiUrlType::ChannelsRoles:
        return QStringLiteral("channels.roles");
    case RestApiUtil::RestApiUrlType::ChannelsCounters:
        return QStringLiteral("channels.counters");
    case RestApiUtil::RestApiUrlType::ChannelsJoin:
        return QStringLiteral("channels.join");
    case RestApiUtil::RestApiUrlType::ChannelsMembers:
        return QStringLiteral("channels.members");
    case RestApiUtil::RestApiUrlType::ChannelsDelete:
        return QStringLiteral("channels.delete");
    case RestApiUtil::RestApiUrlType::ChannelsOnline:
        return QStringLiteral("channels.online");
    case RestApiUtil::RestApiUrlType::ChannelsGetAllUserMentionsByChannel:
        return QStringLiteral("channels.getAllUserMentionsByChannel");
    case RestApiUtil::RestApiUrlType::GroupsAddAll:
        return QStringLiteral("groups.addAll");
    case RestApiUtil::RestApiUrlType::GroupsAddModerator:
        return QStringLiteral("groups.addModerator");
    case RestApiUtil::RestApiUrlType::GroupsAddOwner:
        return QStringLiteral("groups.addOwner");
    case RestApiUtil::RestApiUrlType::GroupsArchive:
        return QStringLiteral("groups.archive");
    case RestApiUtil::RestApiUrlType::GroupsClose:
        return QStringLiteral("groups.close");
    case RestApiUtil::RestApiUrlType::GroupsCreate:
        return QStringLiteral("groups.create");
    case RestApiUtil::RestApiUrlType::GroupsGetIntegrations:
        return QStringLiteral("groups.getIntegrations");
    case RestApiUtil::RestApiUrlType::GroupsHistory:
        return QStringLiteral("groups.history");
    case RestApiUtil::RestApiUrlType::GroupsInfo:
        return QStringLiteral("groups.info");
    case RestApiUtil::RestApiUrlType::GroupsInvite:
        return QStringLiteral("groups.invite");
    case RestApiUtil::RestApiUrlType::GroupsKick:
        return QStringLiteral("groups.kick");
    case RestApiUtil::RestApiUrlType::GroupsLeave:
        return QStringLiteral("groups.leave");
    case RestApiUtil::RestApiUrlType::GroupsList:
        return QStringLiteral("groups.list");
    case RestApiUtil::RestApiUrlType::GroupsOpen:
        return QStringLiteral("groups.open");
    case RestApiUtil::RestApiUrlType::GroupsRemoveModerator:
        return QStringLiteral("groups.removeModerator");
    case RestApiUtil::RestApiUrlType::GroupsRemoveOwner:
        return QStringLiteral("groups.removeOwner");
    case RestApiUtil::RestApiUrlType::GroupsRename:
        return QStringLiteral("groups.rename");
    case RestApiUtil::RestApiUrlType::GroupsSetDescription:
        return QStringLiteral("groups.setDescription");
    case RestApiUtil::RestApiUrlType::GroupsSetPurpose:
        return QStringLiteral("groups.setPurpose");
    case RestApiUtil::RestApiUrlType::GroupsSetReadOnly:
        return QStringLiteral("groups.setReadOnly");
    case RestApiUtil::RestApiUrlType::GroupsSetTopic:
        return QStringLiteral("groups.setTopic");
    case RestApiUtil::RestApiUrlType::GroupsSetType:
        return QStringLiteral("groups.setType");
    case RestApiUtil::RestApiUrlType::GroupsUnarchive:
        return QStringLiteral("groups.unarchive");
    case RestApiUtil::RestApiUrlType::GroupsSetAnnouncement:
        return QStringLiteral("groups.setAnnouncement");
    case RestApiUtil::RestApiUrlType::GroupsFiles:
        return QStringLiteral("groups.files");
    case RestApiUtil::RestApiUrlType::GroupsRoles:
        return QStringLiteral("groups.roles");
    case RestApiUtil::RestApiUrlType::GroupsCounters:
        return QStringLiteral("groups.counters");
    case RestApiUtil::RestApiUrlType::GroupsRemoveLeader:
        return QStringLiteral("groups.removeLeader");
    case RestApiUtil::RestApiUrlType::GroupsAddLeader:
        return QStringLiteral("groups.addLeader");
    case RestApiUtil::RestApiUrlType::GroupsDelete:
        return QStringLiteral("groups.delete");
    case RestApiUtil::RestApiUrlType::GroupsListAll:
        return QStringLiteral("groups.listAll");
    case RestApiUtil::RestApiUrlType::GroupsMembers:
        return QStringLiteral("groups.members");
    case RestApiUtil::RestApiUrlType::GroupsSetEncrypted:
        return QStringLiteral("groups.setEncrypted");
    case RestApiUtil::RestApiUrlType::ServerInfo:
        return QStringLiteral("info");
    case RestApiUtil::RestApiUrlType::Settings:
        return QStringLiteral("settings");
    case RestApiUtil::RestApiUrlType::SettingsPublic:
        return QStringLiteral("settings.public");
    case RestApiUtil::RestApiUrlType::UpdateAdminSettings:
        return QStringLiteral("settings");
    case RestApiUtil::RestApiUrlType::RoomsUpload:
        return QStringLiteral("rooms.upload");
    case RestApiUtil::RestApiUrlType::RoomsSaveNotification:
        return QStringLiteral("rooms.saveNotification");
    case RestApiUtil::RestApiUrlType::RoomsSaveSettings:
        return QStringLiteral("rooms.saveRoomSettings");
    case RestApiUtil::RestApiUrlType::RoomsAdminRooms:
        return QStringLiteral("rooms.adminRooms");
    case RestApiUtil::RestApiUrlType::RoomsAdminRoomsGetRoom:
        return QStringLiteral("rooms.adminRooms.getRoom");
    case RestApiUtil::RestApiUrlType::Spotlight:
        return QStringLiteral("spotlight");
    case RestApiUtil::RestApiUrlType::ImClose:
        return QStringLiteral("im.close");
    case RestApiUtil::RestApiUrlType::ImCreate:
        return QStringLiteral("im.create");
    case RestApiUtil::RestApiUrlType::ImOpen:
        return QStringLiteral("im.open");
    case RestApiUtil::RestApiUrlType::ImSetTopic:
        return QStringLiteral("im.setTopic");
    case RestApiUtil::RestApiUrlType::ImHistory:
        return QStringLiteral("im.history");
    case RestApiUtil::RestApiUrlType::ImFiles:
        return QStringLiteral("im.files");
    case RestApiUtil::RestApiUrlType::ImDelete:
        return QStringLiteral("im.delete");
    case RestApiUtil::RestApiUrlType::ImMessages:
        return QStringLiteral("im.messages");
    case RestApiUtil::RestApiUrlType::ImMembers:
        return QStringLiteral("im.members");
    case RestApiUtil::RestApiUrlType::LoadEmojiCustom:
        return QStringLiteral("emoji-custom.list");
    case RestApiUtil::RestApiUrlType::EmojiCustomDelete:
        return QStringLiteral("emoji-custom.delete");
    case RestApiUtil::RestApiUrlType::EmojiCustomCreate:
        return QStringLiteral("emoji-custom.create");
    case RestApiUtil::RestApiUrlType::EmojiCustomUpdate:
        return QStringLiteral("emoji-custom.update");
    case RestApiUtil::RestApiUrlType::EmojiCustomAll:
        return QStringLiteral("emoji-custom.all");
    case RestApiUtil::RestApiUrlType::SubscriptionsRead:
        return QStringLiteral("subscriptions.read");
    case RestApiUtil::RestApiUrlType::SubscriptionsUnRead:
        return QStringLiteral("subscriptions.unread");
    case RestApiUtil::RestApiUrlType::RoomsGet:
        return QStringLiteral("rooms.get");
    case RestApiUtil::RestApiUrlType::RoomsFavorite:
        return QStringLiteral("rooms.favorite");
    case RestApiUtil::RestApiUrlType::RoomsCleanHistory:
        return QStringLiteral("rooms.cleanHistory");
    // since 1.0.0
    case RestApiUtil::RestApiUrlType::RoomsCreateDiscussion:
        return QStringLiteral("rooms.createDiscussion");
    case RestApiUtil::RestApiUrlType::RoomsGetDiscussions:
        return QStringLiteral("rooms.getDiscussions");

    // since 3.8.0
    case RestApiUtil::RestApiUrlType::RoomsExport:
        return QStringLiteral("rooms.export");
    case RestApiUtil::RestApiUrlType::RoomsChangeArchivactionState:
        return QStringLiteral("rooms.changeArchivationState");
    // since 0.72 ? Need to implement it
    case RestApiUtil::RestApiUrlType::RoomsInfo:
        return QStringLiteral("rooms.info");
    case RestApiUtil::RestApiUrlType::RoomsLeave:
        return QStringLiteral("rooms.leave");
        // Since 5.4.0
    case RestApiUtil::RestApiUrlType::RoomsDelete:
        return QStringLiteral("rooms.delete");

    //
    case RestApiUtil::RestApiUrlType::ForgotPassword:
        return QStringLiteral("users.forgotPassword");
    case RestApiUtil::RestApiUrlType::CommandsList:
        return QStringLiteral("commands.list");
    case RestApiUtil::RestApiUrlType::CommandsGet:
        return QStringLiteral("commands.get");
    case RestApiUtil::RestApiUrlType::CommandsRun:
        return QStringLiteral("commands.run");
    case RestApiUtil::RestApiUrlType::CommandsPreview:
        return QStringLiteral("commands.preview");
    case RestApiUtil::RestApiUrlType::E2EfetchMyKeys:
        return QStringLiteral("e2e.fetchMyKeys");
    case RestApiUtil::RestApiUrlType::E2EupdateGroupKey:
        return QStringLiteral("e2e.updateGroupKey");

    case RestApiUtil::RestApiUrlType::E2ESetRoomKeyID:
        return QStringLiteral("e2e.setRoomKeyID");
    case RestApiUtil::RestApiUrlType::E2ESetUserPublicAndPrivateKeys:
        return QStringLiteral("e2e.setUserPublicAndPrivateKeys");
    case RestApiUtil::RestApiUrlType::E2EGetUsersOfRoomWithoutKey:
        return QStringLiteral("e2e.getUsersOfRoomWithoutKey");
    case RestApiUtil::RestApiUrlType::E2ERequestSubscriptionKeys:
        return QStringLiteral("e2e.requestSubscriptionKeys");
    case RestApiUtil::RestApiUrlType::E2EResetOwnE2EKey:
        return QStringLiteral("e2e.resetOwnE2EKey");

    case RestApiUtil::RestApiUrlType::E2EAcceptSuggestedGroupKey:
        return QStringLiteral("e2e.acceptSuggestedGroupKey");
    case RestApiUtil::RestApiUrlType::E2ERejectSuggestedGroupKey:
        return QStringLiteral("e2e.rejectSuggestedGroupKey");
    case RestApiUtil::RestApiUrlType::E2EProvideUsersWithSuggestedGroupKeys:
        return QStringLiteral("e2e.provideUsersWithSuggestedGroupKeys");
    case RestApiUtil::RestApiUrlType::E2EResetRoomKey:
        return QStringLiteral("e2e.resetRoomKey");
    case RestApiUtil::RestApiUrlType::E2EFetchUsersWaitingForGroupKey:
        return QStringLiteral("e2e.provideUsersSuggestedGroupKeys");

    case RestApiUtil::RestApiUrlType::RolesList:
        return QStringLiteral("roles.list");
    case RestApiUtil::RestApiUrlType::RolesCreate:
        return QStringLiteral("roles.create");
    case RestApiUtil::RestApiUrlType::RolesSync:
        return QStringLiteral("roles.sync");
    case RestApiUtil::RestApiUrlType::RolesUpdate:
        return QStringLiteral("roles.update");
    case RestApiUtil::RestApiUrlType::RolesGetUsersInRole:
        return QStringLiteral("roles.getUsersInRole");
    case RestApiUtil::RestApiUrlType::RolesAddUserToRole:
        return QStringLiteral("roles.addUserToRole");
    case RestApiUtil::RestApiUrlType::RolesRemoveUserFromRole:
        return QStringLiteral("roles.removeUserFromRole");

    case RestApiUtil::RestApiUrlType::RolesDelete:
        return QStringLiteral("roles.delete");

    case RestApiUtil::RestApiUrlType::VideoConfJitsi:
        return QStringLiteral("video-conference/jitsi.update-timeout");

    case RestApiUtil::RestApiUrlType::AutoTranslateGetSupportedLanguages:
        return QStringLiteral("autotranslate.getSupportedLanguages");
    case RestApiUtil::RestApiUrlType::AutoTranslateSaveSettings:
        return QStringLiteral("autotranslate.saveSettings");
    case RestApiUtil::RestApiUrlType::AutoTranslateTranslateMessage:
        return QStringLiteral("autotranslate.translateMessage");

    case RestApiUtil::RestApiUrlType::CustomUserStatusList:
        return QStringLiteral("custom-user-status.list");
    case RestApiUtil::RestApiUrlType::CustomUserStatusCreate:
        return QStringLiteral("custom-user-status.create");
    case RestApiUtil::RestApiUrlType::CustomUserStatusDelete:
        return QStringLiteral("custom-user-status.delete");
    case RestApiUtil::RestApiUrlType::CustomUserStatusUpdate:
        return QStringLiteral("custom-user-status.update");

    case RestApiUtil::RestApiUrlType::CustomSoundsList:
        return QStringLiteral("custom-sounds.list");
    case RestApiUtil::RestApiUrlType::FindOrCreateInvite:
        return QStringLiteral("findOrCreateInvite");
    case RestApiUtil::RestApiUrlType::ListInvites:
        return QStringLiteral("listInvites");
    case RestApiUtil::RestApiUrlType::RemoveInvite:
        return QStringLiteral("removeInvite");
    case RestApiUtil::RestApiUrlType::UseInviteToken:
        return QStringLiteral("useInviteToken");
    case RestApiUtil::RestApiUrlType::ValidateInviteToken:
        return QStringLiteral("validateInviteToken");
    case RestApiUtil::RestApiUrlType::SendInvitationEmails:
        return QStringLiteral("sendInvitationEmail");
    case RestApiUtil::RestApiUrlType::RoomsAutocompleteChannelAndPrivate:
        return QStringLiteral("rooms.autocomplete.channelAndPrivate");
    case RestApiUtil::RestApiUrlType::Users2FASendEmailCode:
        return QStringLiteral("users.2fa.sendEmailCode");
    case RestApiUtil::RestApiUrlType::Users2FAEnableEmail:
        return QStringLiteral("users.2fa.enableEmail");
    case RestApiUtil::RestApiUrlType::Users2FADisableEmail:
        return QStringLiteral("users.2fa.disableEmail");

    case RestApiUtil::RestApiUrlType::UsersDeleteOwnAccount:
        return QStringLiteral("users.deleteOwnAccount");
    case RestApiUtil::RestApiUrlType::UsersSetPreferences:
        return QStringLiteral("users.setPreferences");
    case RestApiUtil::RestApiUrlType::PermissionsListAll:
        return QStringLiteral("permissions.listAll");
    case RestApiUtil::RestApiUrlType::PermissionsUpdate:
        return QStringLiteral("permissions.update");
    case RestApiUtil::RestApiUrlType::Statistics:
        return QStringLiteral("statistics");
    case RestApiUtil::RestApiUrlType::Directory:
        return QStringLiteral("directory");

    case RestApiUtil::RestApiUrlType::TeamsList:
        return QStringLiteral("teams.list");
    case RestApiUtil::RestApiUrlType::TeamsListAll:
        return QStringLiteral("teams.listAll");
    case RestApiUtil::RestApiUrlType::TeamsCreate:
        return QStringLiteral("teams.create");
    case RestApiUtil::RestApiUrlType::TeamsAddRooms:
        return QStringLiteral("teams.addRooms");
    case RestApiUtil::RestApiUrlType::TeamsRemoveRoom:
        return QStringLiteral("teams.removeRoom");
    case RestApiUtil::RestApiUrlType::TeamsUpdateRoom:
        return QStringLiteral("teams.updateRoom");
    case RestApiUtil::RestApiUrlType::TeamsListRooms:
        return QStringLiteral("teams.listRooms");
    case RestApiUtil::RestApiUrlType::TeamsListRoomsOfUser:
        return QStringLiteral("teams.listRoomsOfUser");
    case RestApiUtil::RestApiUrlType::TeamsMembers:
        return QStringLiteral("teams.members");
    case RestApiUtil::RestApiUrlType::TeamsAddMembers:
        return QStringLiteral("teams.addMembers");
    case RestApiUtil::RestApiUrlType::TeamsUpdateMember:
        return QStringLiteral("teams.updateMember");
    case RestApiUtil::RestApiUrlType::TeamsRemoveMember:
        return QStringLiteral("teams.removeMember");
    case RestApiUtil::RestApiUrlType::TeamsLeave:
        return QStringLiteral("teams.leave");
    case RestApiUtil::RestApiUrlType::TeamsInfo:
        return QStringLiteral("teams.info");
    case RestApiUtil::RestApiUrlType::TeamsDelete:
        return QStringLiteral("teams.delete");
    case RestApiUtil::RestApiUrlType::TeamsAutocomplete:
        return QStringLiteral("teams.autocomplete");
    case RestApiUtil::RestApiUrlType::TeamsConvertToChannel:
        return QStringLiteral("teams.convertToChannel");
    case RestApiUtil::RestApiUrlType::RoomsAutocompleteAvailableForTeams:
        return QStringLiteral("rooms.autocomplete.availableForTeams");
    case RestApiUtil::RestApiUrlType::ChannelsConvertToTeam:
        return QStringLiteral("channels.convertToTeam");
    case RestApiUtil::RestApiUrlType::GroupsConvertToTeam:
        return QStringLiteral("groups.convertToTeam");
    case RestApiUtil::RestApiUrlType::StdoutQueue:
        return QStringLiteral("stdout.queue");
    case RestApiUtil::RestApiUrlType::OauthAppsList:
        return QStringLiteral("oauth-apps.list");
    case RestApiUtil::RestApiUrlType::OauthAppsGet:
        return QStringLiteral("oauth-apps.get");
    case RestApiUtil::RestApiUrlType::OauthAppsCreate:
        return QStringLiteral("oauth-apps.create");
    case RestApiUtil::RestApiUrlType::OauthAppsUpdate:
        return QStringLiteral("oauth-apps.update");
    case RestApiUtil::RestApiUrlType::OauthAppsDelete:
        return QStringLiteral("oauth-apps.delete");

    case RestApiUtil::RestApiUrlType::SettingsOauth:
        return QStringLiteral("settings.oauth");
    case RestApiUtil::RestApiUrlType::SettingsAddCustomOauth:
        return QStringLiteral("settings.addCustomOAuth");

    case RestApiUtil::RestApiUrlType::LicensesMaxActiveUsers:
        return QStringLiteral("licenses.maxActiveUsers");
    case RestApiUtil::RestApiUrlType::LicensesAdd:
        return QStringLiteral("licenses.add");
    case RestApiUtil::RestApiUrlType::LicensesIsEntreprise:
        return QStringLiteral("licenses.isEnterprise");
    case RestApiUtil::RestApiUrlType::LicensesInfo:
        return QStringLiteral("licenses.info");

    case RestApiUtil::RestApiUrlType::BannersDismiss:
        return QStringLiteral("banners.dismiss");
    case RestApiUtil::RestApiUrlType::Banners:
        return QStringLiteral("banners");

    // RC 5.0
    case RestApiUtil::RestApiUrlType::SessionsList:
        return QStringLiteral("sessions/list");
    case RestApiUtil::RestApiUrlType::SessionsInfo:
        return QStringLiteral("sessions/info");
    case RestApiUtil::RestApiUrlType::SessionsListAll:
        return QStringLiteral("sessions/list.all");
    case RestApiUtil::RestApiUrlType::SessionsLogoutMe:
        return QStringLiteral("sessions/logout.me");
    case RestApiUtil::RestApiUrlType::SessionsLogout:
        return QStringLiteral("sessions/logout");
    case RestApiUtil::RestApiUrlType::SessionsInfoAdmin:
        return QStringLiteral("sessions/info.admin");

    case RestApiUtil::RestApiUrlType::UserRequestDataDownload:
        return QStringLiteral("users.requestDataDownload");

    case RestApiUtil::RestApiUrlType::VideoConferenceStart:
        return QStringLiteral("video-conference.start");
    case RestApiUtil::RestApiUrlType::VideoConferenceJoin:
        return QStringLiteral("video-conference.join");
    case RestApiUtil::RestApiUrlType::VideoConferenceCancel:
        return QStringLiteral("video-conference.cancel");
    case RestApiUtil::RestApiUrlType::VideoConferenceInfo:
        return QStringLiteral("video-conference.info");
    case RestApiUtil::RestApiUrlType::VideoConferenceList:
        return QStringLiteral("video-conference.list");
    case RestApiUtil::RestApiUrlType::VideoConferenceProviders:
        return QStringLiteral("video-conference.providers");
    case RestApiUtil::RestApiUrlType::VideoConferenceCapabilities:
        return QStringLiteral("video-conference.capabilities");
    case RestApiUtil::RestApiUrlType::ModerationReportsByUsers:
        return QStringLiteral("moderation.reportsByUsers");
    case RestApiUtil::RestApiUrlType::ModerationDismissReports:
        return QStringLiteral("moderation.dismissReports");
    case RestApiUtil::RestApiUrlType::ModerationUserReportedMessages:
        return QStringLiteral("moderation.user.reportedMessages");
    case RestApiUtil::RestApiUrlType::ModerationUserDeleteReportedMessages:
        return QStringLiteral("moderation.user.deleteReportedMessages");
    case RestApiUtil::RestApiUrlType::ModerationReports:
        return QStringLiteral("moderation.reports");
    case RestApiUtil::RestApiUrlType::ModerationReportInfo:
        return QStringLiteral("moderation.reportInfo");
    case RestApiUtil::RestApiUrlType::ModerationUserReports:
        return QStringLiteral("moderation.userReports");
    case RestApiUtil::RestApiUrlType::ModerationUserReportsByUserId:
        return QStringLiteral("moderation.user.reportsByUserId");
    case RestApiUtil::RestApiUrlType::ModerationDismissUserReports:
        return QStringLiteral("moderation.dismissUserReports");
    case RestApiUtil::RestApiUrlType::ModerationReportUser:
        return QStringLiteral("moderation.reportUser");

    case RestApiUtil::RestApiUrlType::RoomsNameExists:
        return QStringLiteral("rooms.nameExists");
    case RestApiUtil::RestApiUrlType::MethodCall:
        return QStringLiteral("method.call");
    case RestApiUtil::RestApiUrlType::MethodCallAnon:
        return QStringLiteral("method.callAnon");
    case RestApiUtil::RestApiUrlType::AppsUiInteraction:
        return QStringLiteral("ui.interaction");

    case RestApiUtil::RestApiUrlType::UsersLogoutOtherClients:
        return QStringLiteral("users.logoutOtherClients");

    // 6.8.0
    case RestApiUtil::RestApiUrlType::RoomsMuteUser:
        return QStringLiteral("rooms.muteUser");
    case RestApiUtil::RestApiUrlType::RoomsUnmuteUser:
        return QStringLiteral("rooms.unmuteUser");

    case RestApiUtil::RestApiUrlType::FeaturedApps:
        return QStringLiteral("featured-apps");
    case RestApiUtil::RestApiUrlType::CategoriesApps:
        return QStringLiteral("categories");
    case RestApiUtil::RestApiUrlType::CountApps:
        return QStringLiteral("count");
    case RestApiUtil::RestApiUrlType::NotifyAdminsApps:
        return QStringLiteral("notify-admins");
    case RestApiUtil::RestApiUrlType::InstalledApps:
        return QStringLiteral("installed");
    case RestApiUtil::RestApiUrlType::MarketplaceApps:
        return QStringLiteral("marketplace");

    case RestApiUtil::RestApiUrlType::UsersListByStatus:
        return QStringLiteral("users.listByStatus");

    case RestApiUtil::RestApiUrlType::UsersSendWelcomeEmail:
        return QStringLiteral("users.sendWelcomeEmail");
    case RestApiUtil::RestApiUrlType::Empty:
        return {};
    case RestApiUtil::RestApiUrlType::RoomsImages:
        return QStringLiteral("rooms.images");
    case RestApiUtil::RestApiUrlType::RoomsMembersOrderedByRole:
        return QStringLiteral("rooms.membersOrderedByRole");
    }
    qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Unknown RestApiUtil::RestApiUrlType " << static_cast<int>(type);
    return {};
}

QByteArray RestApiUtil::convertSha256Password(const QString &pwd)
{
    const QByteArray sha256pw = QCryptographicHash::hash(pwd.toUtf8(), QCryptographicHash::Sha256);
    return sha256pw.toHex();
}

QString RestApiUtil::restApiUrlExtensionType(RestApiUrlExtensionType type)
{
    switch (type) {
    case RestApiUrlExtensionType::V1:
        return QStringLiteral("/api/v1/");
    case RestApiUrlExtensionType::Apps:
        return QStringLiteral("/api/apps/");
    case RestApiUtil::RestApiUrlExtensionType::NoExtension:
        return QStringLiteral("/api/");
    }
    return {};
}
