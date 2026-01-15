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
        return u"https://"_s + url;
    }
}

QString RestApiUtil::restUrl(RestApiUtil::RestApiUrlType type)
{
    switch (type) {
    case RestApiUtil::RestApiUrlType::Login:
        return u"login"_s;
    case RestApiUtil::RestApiUrlType::Logout:
        return u"logout"_s;
    case RestApiUtil::RestApiUrlType::Me:
        return u"me"_s;
    case RestApiUtil::RestApiUrlType::UsersGetAvatar:
        return u"users.getAvatar"_s;
    case RestApiUtil::RestApiUrlType::UsersDelete:
        return u"users.delete"_s;
    case RestApiUtil::RestApiUrlType::UsersCreate:
        return u"users.create"_s;
    case RestApiUtil::RestApiUrlType::UsersCreateToken:
        return u"users.createToken"_s;
    case RestApiUtil::RestApiUrlType::UsersGetPresence:
        return u"users.getPresence"_s;
    case RestApiUtil::RestApiUrlType::UsersInfo:
        return u"users.info"_s;
    case RestApiUtil::RestApiUrlType::UsersList:
        return u"users.list"_s;
    case RestApiUtil::RestApiUrlType::UsersRegister:
        return u"users.register"_s;
    case RestApiUtil::RestApiUrlType::UsersResetAvatar:
        return u"users.resetAvatar"_s;
    case RestApiUtil::RestApiUrlType::UsersSetAvatar:
        return u"users.setAvatar"_s;
    case RestApiUtil::RestApiUrlType::UsersUpdate:
        return u"users.update"_s;
    case RestApiUtil::RestApiUrlType::UsersGetUsernameSuggestion:
        return u"users.getUsernameSuggestion"_s;

    case RestApiUtil::RestApiUrlType::UsersRemovePersonalAccessToken:
        return u"users.removePersonalAccessToken"_s;
    case RestApiUtil::RestApiUrlType::UsersGeneratePersonalAccessToken:
        return u"users.generatePersonalAccessToken"_s;
    case RestApiUtil::RestApiUrlType::UsersRegeneratePersonalAccessToken:
        return u"users.regeneratePersonalAccessToken"_s;
    case RestApiUtil::RestApiUrlType::UsersGetPersonalAccessTokens:
        return u"users.getPersonalAccessTokens"_s;

    case RestApiUtil::RestApiUrlType::UsersPresence:
        return u"users.presence"_s;
    case RestApiUtil::RestApiUrlType::UsersUpdateOwnBasicInfo:
        return u"users.updateOwnBasicInfo"_s;
    case RestApiUtil::RestApiUrlType::UsersSetStatus:
        return u"users.setStatus"_s;
    case RestApiUtil::RestApiUrlType::UsersGetStatus:
        return u"users.getStatus"_s;
    case RestApiUtil::RestApiUrlType::UsersAutocomplete:
        return u"users.autocomplete"_s;
    case RestApiUtil::RestApiUrlType::UsersRemoveOtherTokens:
        return u"users.removeOtherTokens"_s;
    case RestApiUtil::RestApiUrlType::UsersSetActiveStatus:
        return u"users.setActiveStatus"_s;
    case RestApiUtil::RestApiUrlType::UsersResetTOTP:
        return u"users.resetTOTP"_s;
    case RestApiUtil::RestApiUrlType::UsersResetE2EKey:
        return u"users.resetE2EKey"_s;

    case RestApiUtil::RestApiUrlType::ChatDelete:
        return u"chat.delete"_s;
    case RestApiUtil::RestApiUrlType::ChatGetMessage:
        return u"chat.getMessage"_s;
    case RestApiUtil::RestApiUrlType::ChatPinMessage:
        return u"chat.pinMessage"_s;
    case RestApiUtil::RestApiUrlType::ChatPostMessage:
        return u"chat.postMessage"_s;
    case RestApiUtil::RestApiUrlType::ChatReact:
        return u"chat.react"_s;
    case RestApiUtil::RestApiUrlType::ChatStarMessage:
        return u"chat.starMessage"_s;
    case RestApiUtil::RestApiUrlType::ChatUnPinMessage:
        return u"chat.unPinMessage"_s;
    case RestApiUtil::RestApiUrlType::ChatUnStarMessage:
        return u"chat.unStarMessage"_s;
    case RestApiUtil::RestApiUrlType::ChatUpdate:
        return u"chat.update"_s;
    case RestApiUtil::RestApiUrlType::ChatSearch:
        return u"chat.search"_s;
    case RestApiUtil::RestApiUrlType::ChatIgnoreUser:
        return u"chat.ignoreUser"_s;
    case RestApiUtil::RestApiUrlType::ChatReportMessage:
        return u"chat.reportMessage"_s;
    case RestApiUtil::RestApiUrlType::ChatFollowMessage:
        return u"chat.followMessage"_s;
    case RestApiUtil::RestApiUrlType::ChatUnFollowMessage:
        return u"chat.unfollowMessage"_s;
    case RestApiUtil::RestApiUrlType::ChatGetDeletedMessage:
        return u"chat.getDeletedMessages"_s;
    case RestApiUtil::RestApiUrlType::ChatMessageReadReceipts:
        return u"chat.getMessageReadReceipts"_s;
    case RestApiUtil::RestApiUrlType::ChatSyncThreadsList:
        return u"chat.syncThreadsList"_s;
    case RestApiUtil::RestApiUrlType::ChatGetThreadsList:
        return u"chat.getThreadsList"_s;
    case RestApiUtil::RestApiUrlType::ChatSyncThreadMessages:
        return u"chat.syncThreadMessages"_s;
    case RestApiUtil::RestApiUrlType::ChatGetThreadMessages:
        return u"chat.getThreadMessages"_s;
    case RestApiUtil::RestApiUrlType::ChatSendMessage:
        return u"chat.sendMessage"_s;
    case RestApiUtil::RestApiUrlType::ChatGetPinnedMessages:
        return u"chat.getPinnedMessages"_s;
    case RestApiUtil::RestApiUrlType::ChatGetMentionedMessages:
        return u"chat.getMentionedMessages"_s;
    case RestApiUtil::RestApiUrlType::ChatGetStarredMessages:
        return u"chat.getStarredMessages"_s;
    case RestApiUtil::RestApiUrlType::ChatGetSnippetedMessages:
        return u"chat.getSnippetedMessages"_s;
    case RestApiUtil::RestApiUrlType::ChatSyncMessages:
        return u"chat.syncMessages"_s;
    case RestApiUtil::RestApiUrlType::ChannelsAddAll:
        return u"channels.addAll"_s;
    case RestApiUtil::RestApiUrlType::ChannelsAddModerator:
        return u"channels.addModerator"_s;
    case RestApiUtil::RestApiUrlType::ChannelsAddOwner:
        return u"channels.addOwner"_s;
    case RestApiUtil::RestApiUrlType::ChannelsAddLeader:
        return u"channels.addLeader"_s;
    case RestApiUtil::RestApiUrlType::ChannelsArchive:
        return u"channels.archive"_s;
    case RestApiUtil::RestApiUrlType::ChannelsModerators:
        return u"channels.moderators"_s;
    case RestApiUtil::RestApiUrlType::ChannelsClose:
        return u"channels.close"_s;
    case RestApiUtil::RestApiUrlType::ChannelsCreate:
        return u"channels.create"_s;
    case RestApiUtil::RestApiUrlType::ChannelsGetIntegrations:
        return u"channels.getIntegrations"_s;
    case RestApiUtil::RestApiUrlType::ChannelsHistory:
        return u"channels.history"_s;
    case RestApiUtil::RestApiUrlType::ChannelsInfo:
        return u"channels.info"_s;
    case RestApiUtil::RestApiUrlType::ChannelsInvite:
        return u"channels.invite"_s;
    case RestApiUtil::RestApiUrlType::ChannelsKick:
        return u"channels.kick"_s;
    case RestApiUtil::RestApiUrlType::ChannelsLeave:
        return u"channels.leave"_s;
    case RestApiUtil::RestApiUrlType::ChannelsList:
        return u"channels.list"_s;
    case RestApiUtil::RestApiUrlType::ChannelsListJoined:
        return u"channels.list.joined"_s;
    case RestApiUtil::RestApiUrlType::ChannelsOpen:
        return u"channels.open"_s;
    case RestApiUtil::RestApiUrlType::ChannelsRemoveModerator:
        return u"channels.removeModerator"_s;
    case RestApiUtil::RestApiUrlType::ChannelsRemoveOwner:
        return u"channels.removeOwner"_s;
    case RestApiUtil::RestApiUrlType::ChannelsRemoveLeader:
        return u"channels.removeLeader"_s;
    case RestApiUtil::RestApiUrlType::ChannelsRename:
        return u"channels.rename"_s;
    case RestApiUtil::RestApiUrlType::ChannelsSetDescription:
        return u"channels.setDescription"_s;
    case RestApiUtil::RestApiUrlType::ChannelsSetJoinCode:
        return u"channels.setJoinCode"_s;
    case RestApiUtil::RestApiUrlType::ChannelsSetPurpose:
        return u"channels.setPurpose"_s;
    case RestApiUtil::RestApiUrlType::ChannelsSetReadOnly:
        return u"channels.setReadOnly"_s;
    case RestApiUtil::RestApiUrlType::ChannelsSetTopic:
        return u"channels.setTopic"_s;
    case RestApiUtil::RestApiUrlType::ChannelsSetAnnouncement:
        return u"channels.setAnnouncement"_s;
    case RestApiUtil::RestApiUrlType::ChannelsFiles:
        return u"channels.files"_s;
    case RestApiUtil::RestApiUrlType::ChannelsSetType:
        return u"channels.setType"_s;
    case RestApiUtil::RestApiUrlType::ChannelsUnarchive:
        return u"channels.unarchive"_s;
    case RestApiUtil::RestApiUrlType::ChannelsRoles:
        return u"channels.roles"_s;
    case RestApiUtil::RestApiUrlType::ChannelsCounters:
        return u"channels.counters"_s;
    case RestApiUtil::RestApiUrlType::ChannelsJoin:
        return u"channels.join"_s;
    case RestApiUtil::RestApiUrlType::ChannelsMembers:
        return u"channels.members"_s;
    case RestApiUtil::RestApiUrlType::ChannelsDelete:
        return u"channels.delete"_s;
    case RestApiUtil::RestApiUrlType::ChannelsOnline:
        return u"channels.online"_s;
    case RestApiUtil::RestApiUrlType::ChannelsGetAllUserMentionsByChannel:
        return u"channels.getAllUserMentionsByChannel"_s;
    case RestApiUtil::RestApiUrlType::GroupsAddAll:
        return u"groups.addAll"_s;
    case RestApiUtil::RestApiUrlType::GroupsAddModerator:
        return u"groups.addModerator"_s;
    case RestApiUtil::RestApiUrlType::GroupsAddOwner:
        return u"groups.addOwner"_s;
    case RestApiUtil::RestApiUrlType::GroupsArchive:
        return u"groups.archive"_s;
    case RestApiUtil::RestApiUrlType::GroupsClose:
        return u"groups.close"_s;
    case RestApiUtil::RestApiUrlType::GroupsCreate:
        return u"groups.create"_s;
    case RestApiUtil::RestApiUrlType::GroupsGetIntegrations:
        return u"groups.getIntegrations"_s;
    case RestApiUtil::RestApiUrlType::GroupsHistory:
        return u"groups.history"_s;
    case RestApiUtil::RestApiUrlType::GroupsInfo:
        return u"groups.info"_s;
    case RestApiUtil::RestApiUrlType::GroupsInvite:
        return u"groups.invite"_s;
    case RestApiUtil::RestApiUrlType::GroupsKick:
        return u"groups.kick"_s;
    case RestApiUtil::RestApiUrlType::GroupsLeave:
        return u"groups.leave"_s;
    case RestApiUtil::RestApiUrlType::GroupsList:
        return u"groups.list"_s;
    case RestApiUtil::RestApiUrlType::GroupsOpen:
        return u"groups.open"_s;
    case RestApiUtil::RestApiUrlType::GroupsRemoveModerator:
        return u"groups.removeModerator"_s;
    case RestApiUtil::RestApiUrlType::GroupsRemoveOwner:
        return u"groups.removeOwner"_s;
    case RestApiUtil::RestApiUrlType::GroupsRename:
        return u"groups.rename"_s;
    case RestApiUtil::RestApiUrlType::GroupsSetDescription:
        return u"groups.setDescription"_s;
    case RestApiUtil::RestApiUrlType::GroupsSetPurpose:
        return u"groups.setPurpose"_s;
    case RestApiUtil::RestApiUrlType::GroupsSetReadOnly:
        return u"groups.setReadOnly"_s;
    case RestApiUtil::RestApiUrlType::GroupsSetTopic:
        return u"groups.setTopic"_s;
    case RestApiUtil::RestApiUrlType::GroupsSetType:
        return u"groups.setType"_s;
    case RestApiUtil::RestApiUrlType::GroupsUnarchive:
        return u"groups.unarchive"_s;
    case RestApiUtil::RestApiUrlType::GroupsSetAnnouncement:
        return u"groups.setAnnouncement"_s;
    case RestApiUtil::RestApiUrlType::GroupsFiles:
        return u"groups.files"_s;
    case RestApiUtil::RestApiUrlType::GroupsRoles:
        return u"groups.roles"_s;
    case RestApiUtil::RestApiUrlType::GroupsCounters:
        return u"groups.counters"_s;
    case RestApiUtil::RestApiUrlType::GroupsRemoveLeader:
        return u"groups.removeLeader"_s;
    case RestApiUtil::RestApiUrlType::GroupsAddLeader:
        return u"groups.addLeader"_s;
    case RestApiUtil::RestApiUrlType::GroupsDelete:
        return u"groups.delete"_s;
    case RestApiUtil::RestApiUrlType::GroupsListAll:
        return u"groups.listAll"_s;
    case RestApiUtil::RestApiUrlType::GroupsMembers:
        return u"groups.members"_s;
    case RestApiUtil::RestApiUrlType::GroupsSetEncrypted:
        return u"groups.setEncrypted"_s;
    case RestApiUtil::RestApiUrlType::ServerInfo:
        return u"info"_s;
    case RestApiUtil::RestApiUrlType::Settings:
        return u"settings"_s;
    case RestApiUtil::RestApiUrlType::SettingsPublic:
        return u"settings.public"_s;
    case RestApiUtil::RestApiUrlType::UpdateAdminSettings:
        return u"settings"_s;
    case RestApiUtil::RestApiUrlType::RoomsUpload:
        return u"rooms.upload"_s;
    case RestApiUtil::RestApiUrlType::RoomsMedia:
        return u"rooms.media"_s;
    case RestApiUtil::RestApiUrlType::RoomsMediaConfirm:
        return u"rooms.mediaConfirm"_s;
    case RestApiUtil::RestApiUrlType::RoomsSaveNotification:
        return u"rooms.saveNotification"_s;
    case RestApiUtil::RestApiUrlType::RoomsSaveSettings:
        return u"rooms.saveRoomSettings"_s;
    case RestApiUtil::RestApiUrlType::RoomsAdminRooms:
        return u"rooms.adminRooms"_s;
    case RestApiUtil::RestApiUrlType::RoomsAdminRoomsGetRoom:
        return u"rooms.adminRooms.getRoom"_s;
    case RestApiUtil::RestApiUrlType::Spotlight:
        return u"spotlight"_s;
    case RestApiUtil::RestApiUrlType::ImClose:
        return u"im.close"_s;
    case RestApiUtil::RestApiUrlType::ImCreate:
        return u"im.create"_s;
    case RestApiUtil::RestApiUrlType::ImOpen:
        return u"im.open"_s;
    case RestApiUtil::RestApiUrlType::ImSetTopic:
        return u"im.setTopic"_s;
    case RestApiUtil::RestApiUrlType::ImHistory:
        return u"im.history"_s;
    case RestApiUtil::RestApiUrlType::ImFiles:
        return u"im.files"_s;
    case RestApiUtil::RestApiUrlType::ImDelete:
        return u"im.delete"_s;
    case RestApiUtil::RestApiUrlType::ImMessages:
        return u"im.messages"_s;
    case RestApiUtil::RestApiUrlType::ImMembers:
        return u"im.members"_s;
    case RestApiUtil::RestApiUrlType::LoadEmojiCustom:
        return u"emoji-custom.list"_s;
    case RestApiUtil::RestApiUrlType::EmojiCustomDelete:
        return u"emoji-custom.delete"_s;
    case RestApiUtil::RestApiUrlType::EmojiCustomCreate:
        return u"emoji-custom.create"_s;
    case RestApiUtil::RestApiUrlType::EmojiCustomUpdate:
        return u"emoji-custom.update"_s;
    case RestApiUtil::RestApiUrlType::EmojiCustomAll:
        return u"emoji-custom.all"_s;
    case RestApiUtil::RestApiUrlType::SubscriptionsRead:
        return u"subscriptions.read"_s;
    case RestApiUtil::RestApiUrlType::SubscriptionsUnRead:
        return u"subscriptions.unread"_s;
    case RestApiUtil::RestApiUrlType::RoomsGet:
        return u"rooms.get"_s;
    case RestApiUtil::RestApiUrlType::RoomsFavorite:
        return u"rooms.favorite"_s;
    case RestApiUtil::RestApiUrlType::RoomsCleanHistory:
        return u"rooms.cleanHistory"_s;
    // since 1.0.0
    case RestApiUtil::RestApiUrlType::RoomsCreateDiscussion:
        return u"rooms.createDiscussion"_s;
    case RestApiUtil::RestApiUrlType::RoomsGetDiscussions:
        return u"rooms.getDiscussions"_s;

    // since 3.8.0
    case RestApiUtil::RestApiUrlType::RoomsExport:
        return u"rooms.export"_s;
    case RestApiUtil::RestApiUrlType::RoomsChangeArchivactionState:
        return u"rooms.changeArchivationState"_s;
    // since 0.72 ? Need to implement it
    case RestApiUtil::RestApiUrlType::RoomsInfo:
        return u"rooms.info"_s;
    case RestApiUtil::RestApiUrlType::RoomsLeave:
        return u"rooms.leave"_s;
        // Since 5.4.0
    case RestApiUtil::RestApiUrlType::RoomsDelete:
        return u"rooms.delete"_s;

    //
    case RestApiUtil::RestApiUrlType::ForgotPassword:
        return u"users.forgotPassword"_s;
    case RestApiUtil::RestApiUrlType::CommandsList:
        return u"commands.list"_s;
    case RestApiUtil::RestApiUrlType::CommandsGet:
        return u"commands.get"_s;
    case RestApiUtil::RestApiUrlType::CommandsRun:
        return u"commands.run"_s;
    case RestApiUtil::RestApiUrlType::CommandsPreview:
        return u"commands.preview"_s;
    case RestApiUtil::RestApiUrlType::E2EfetchMyKeys:
        return u"e2e.fetchMyKeys"_s;
    case RestApiUtil::RestApiUrlType::E2EupdateGroupKey:
        return u"e2e.updateGroupKey"_s;

    case RestApiUtil::RestApiUrlType::E2ESetRoomKeyID:
        return u"e2e.setRoomKeyID"_s;
    case RestApiUtil::RestApiUrlType::E2ESetUserPublicAndPrivateKeys:
        return u"e2e.setUserPublicAndPrivateKeys"_s;
    case RestApiUtil::RestApiUrlType::E2EGetUsersOfRoomWithoutKey:
        return u"e2e.getUsersOfRoomWithoutKey"_s;
    case RestApiUtil::RestApiUrlType::E2ERequestSubscriptionKeys:
        return u"e2e.requestSubscriptionKeys"_s;
    case RestApiUtil::RestApiUrlType::E2EResetOwnE2EKey:
        return u"e2e.resetOwnE2EKey"_s;

    case RestApiUtil::RestApiUrlType::E2EAcceptSuggestedGroupKey:
        return u"e2e.acceptSuggestedGroupKey"_s;
    case RestApiUtil::RestApiUrlType::E2ERejectSuggestedGroupKey:
        return u"e2e.rejectSuggestedGroupKey"_s;
    case RestApiUtil::RestApiUrlType::E2EProvideUsersWithSuggestedGroupKeys:
        return u"e2e.provideUsersWithSuggestedGroupKeys"_s;
    case RestApiUtil::RestApiUrlType::E2EResetRoomKey:
        return u"e2e.resetRoomKey"_s;
    case RestApiUtil::RestApiUrlType::E2EFetchUsersWaitingForGroupKey:
        return u"e2e.provideUsersSuggestedGroupKeys"_s;

    case RestApiUtil::RestApiUrlType::RolesList:
        return u"roles.list"_s;
    case RestApiUtil::RestApiUrlType::RolesCreate:
        return u"roles.create"_s;
    case RestApiUtil::RestApiUrlType::RolesSync:
        return u"roles.sync"_s;
    case RestApiUtil::RestApiUrlType::RolesUpdate:
        return u"roles.update"_s;
    case RestApiUtil::RestApiUrlType::RolesGetUsersInRole:
        return u"roles.getUsersInRole"_s;
    case RestApiUtil::RestApiUrlType::RolesAddUserToRole:
        return u"roles.addUserToRole"_s;
    case RestApiUtil::RestApiUrlType::RolesRemoveUserFromRole:
        return u"roles.removeUserFromRole"_s;

    case RestApiUtil::RestApiUrlType::RolesDelete:
        return u"roles.delete"_s;

    case RestApiUtil::RestApiUrlType::VideoConfJitsi:
        return u"video-conference/jitsi.update-timeout"_s;

    case RestApiUtil::RestApiUrlType::AutoTranslateGetSupportedLanguages:
        return u"autotranslate.getSupportedLanguages"_s;
    case RestApiUtil::RestApiUrlType::AutoTranslateSaveSettings:
        return u"autotranslate.saveSettings"_s;
    case RestApiUtil::RestApiUrlType::AutoTranslateTranslateMessage:
        return u"autotranslate.translateMessage"_s;

    case RestApiUtil::RestApiUrlType::CustomUserStatusList:
        return u"custom-user-status.list"_s;
    case RestApiUtil::RestApiUrlType::CustomUserStatusCreate:
        return u"custom-user-status.create"_s;
    case RestApiUtil::RestApiUrlType::CustomUserStatusDelete:
        return u"custom-user-status.delete"_s;
    case RestApiUtil::RestApiUrlType::CustomUserStatusUpdate:
        return u"custom-user-status.update"_s;

    case RestApiUtil::RestApiUrlType::CustomSoundsList:
        return u"custom-sounds.list"_s;
    case RestApiUtil::RestApiUrlType::FindOrCreateInvite:
        return u"findOrCreateInvite"_s;
    case RestApiUtil::RestApiUrlType::ListInvites:
        return u"listInvites"_s;
    case RestApiUtil::RestApiUrlType::RemoveInvite:
        return u"removeInvite"_s;
    case RestApiUtil::RestApiUrlType::UseInviteToken:
        return u"useInviteToken"_s;
    case RestApiUtil::RestApiUrlType::ValidateInviteToken:
        return u"validateInviteToken"_s;
    case RestApiUtil::RestApiUrlType::SendInvitationEmails:
        return u"sendInvitationEmail"_s;
    case RestApiUtil::RestApiUrlType::RoomsAutocompleteChannelAndPrivate:
        return u"rooms.autocomplete.channelAndPrivate"_s;
    case RestApiUtil::RestApiUrlType::Users2FASendEmailCode:
        return u"users.2fa.sendEmailCode"_s;
    case RestApiUtil::RestApiUrlType::Users2FAEnableEmail:
        return u"users.2fa.enableEmail"_s;
    case RestApiUtil::RestApiUrlType::Users2FADisableEmail:
        return u"users.2fa.disableEmail"_s;

    case RestApiUtil::RestApiUrlType::UsersDeleteOwnAccount:
        return u"users.deleteOwnAccount"_s;
    case RestApiUtil::RestApiUrlType::UsersSetPreferences:
        return u"users.setPreferences"_s;
    case RestApiUtil::RestApiUrlType::PermissionsListAll:
        return u"permissions.listAll"_s;
    case RestApiUtil::RestApiUrlType::PermissionsUpdate:
        return u"permissions.update"_s;
    case RestApiUtil::RestApiUrlType::Statistics:
        return u"statistics"_s;
    case RestApiUtil::RestApiUrlType::Directory:
        return u"directory"_s;

    case RestApiUtil::RestApiUrlType::TeamsList:
        return u"teams.list"_s;
    case RestApiUtil::RestApiUrlType::TeamsListAll:
        return u"teams.listAll"_s;
    case RestApiUtil::RestApiUrlType::TeamsCreate:
        return u"teams.create"_s;
    case RestApiUtil::RestApiUrlType::TeamsAddRooms:
        return u"teams.addRooms"_s;
    case RestApiUtil::RestApiUrlType::TeamsRemoveRoom:
        return u"teams.removeRoom"_s;
    case RestApiUtil::RestApiUrlType::TeamsUpdateRoom:
        return u"teams.updateRoom"_s;
    case RestApiUtil::RestApiUrlType::TeamsListRooms:
        return u"teams.listRooms"_s;
    case RestApiUtil::RestApiUrlType::TeamsListRoomsOfUser:
        return u"teams.listRoomsOfUser"_s;
    case RestApiUtil::RestApiUrlType::TeamsMembers:
        return u"teams.members"_s;
    case RestApiUtil::RestApiUrlType::TeamsAddMembers:
        return u"teams.addMembers"_s;
    case RestApiUtil::RestApiUrlType::TeamsUpdateMember:
        return u"teams.updateMember"_s;
    case RestApiUtil::RestApiUrlType::TeamsRemoveMember:
        return u"teams.removeMember"_s;
    case RestApiUtil::RestApiUrlType::TeamsLeave:
        return u"teams.leave"_s;
    case RestApiUtil::RestApiUrlType::TeamsInfo:
        return u"teams.info"_s;
    case RestApiUtil::RestApiUrlType::TeamsDelete:
        return u"teams.delete"_s;
    case RestApiUtil::RestApiUrlType::TeamsAutocomplete:
        return u"teams.autocomplete"_s;
    case RestApiUtil::RestApiUrlType::TeamsConvertToChannel:
        return u"teams.convertToChannel"_s;
    case RestApiUtil::RestApiUrlType::RoomsAutocompleteAvailableForTeams:
        return u"rooms.autocomplete.availableForTeams"_s;
    case RestApiUtil::RestApiUrlType::ChannelsConvertToTeam:
        return u"channels.convertToTeam"_s;
    case RestApiUtil::RestApiUrlType::GroupsConvertToTeam:
        return u"groups.convertToTeam"_s;
    case RestApiUtil::RestApiUrlType::StdoutQueue:
        return u"stdout.queue"_s;
    case RestApiUtil::RestApiUrlType::OauthAppsList:
        return u"oauth-apps.list"_s;
    case RestApiUtil::RestApiUrlType::OauthAppsGet:
        return u"oauth-apps.get"_s;
    case RestApiUtil::RestApiUrlType::OauthAppsCreate:
        return u"oauth-apps.create"_s;
    case RestApiUtil::RestApiUrlType::OauthAppsUpdate:
        return u"oauth-apps.update"_s;
    case RestApiUtil::RestApiUrlType::OauthAppsDelete:
        return u"oauth-apps.delete"_s;

    case RestApiUtil::RestApiUrlType::SettingsOauth:
        return u"settings.oauth"_s;
    case RestApiUtil::RestApiUrlType::SettingsAddCustomOauth:
        return u"settings.addCustomOAuth"_s;

    case RestApiUtil::RestApiUrlType::LicensesMaxActiveUsers:
        return u"licenses.maxActiveUsers"_s;
    case RestApiUtil::RestApiUrlType::LicensesAdd:
        return u"licenses.add"_s;
    case RestApiUtil::RestApiUrlType::LicensesIsEntreprise:
        return u"licenses.isEnterprise"_s;
    case RestApiUtil::RestApiUrlType::LicensesInfo:
        return u"licenses.info"_s;

    case RestApiUtil::RestApiUrlType::BannersDismiss:
        return u"banners.dismiss"_s;
    case RestApiUtil::RestApiUrlType::Banners:
        return u"banners"_s;

    // RC 5.0
    case RestApiUtil::RestApiUrlType::SessionsList:
        return u"sessions/list"_s;
    case RestApiUtil::RestApiUrlType::SessionsInfo:
        return u"sessions/info"_s;
    case RestApiUtil::RestApiUrlType::SessionsListAll:
        return u"sessions/list.all"_s;
    case RestApiUtil::RestApiUrlType::SessionsLogoutMe:
        return u"sessions/logout.me"_s;
    case RestApiUtil::RestApiUrlType::SessionsLogout:
        return u"sessions/logout"_s;
    case RestApiUtil::RestApiUrlType::SessionsInfoAdmin:
        return u"sessions/info.admin"_s;

    case RestApiUtil::RestApiUrlType::UserRequestDataDownload:
        return u"users.requestDataDownload"_s;

    case RestApiUtil::RestApiUrlType::VideoConferenceStart:
        return u"video-conference.start"_s;
    case RestApiUtil::RestApiUrlType::VideoConferenceJoin:
        return u"video-conference.join"_s;
    case RestApiUtil::RestApiUrlType::VideoConferenceCancel:
        return u"video-conference.cancel"_s;
    case RestApiUtil::RestApiUrlType::VideoConferenceInfo:
        return u"video-conference.info"_s;
    case RestApiUtil::RestApiUrlType::VideoConferenceList:
        return u"video-conference.list"_s;
    case RestApiUtil::RestApiUrlType::VideoConferenceProviders:
        return u"video-conference.providers"_s;
    case RestApiUtil::RestApiUrlType::VideoConferenceCapabilities:
        return u"video-conference.capabilities"_s;
    case RestApiUtil::RestApiUrlType::ModerationReportsByUsers:
        return u"moderation.reportsByUsers"_s;
    case RestApiUtil::RestApiUrlType::ModerationDismissReports:
        return u"moderation.dismissReports"_s;
    case RestApiUtil::RestApiUrlType::ModerationUserReportedMessages:
        return u"moderation.user.reportedMessages"_s;
    case RestApiUtil::RestApiUrlType::ModerationUserDeleteReportedMessages:
        return u"moderation.user.deleteReportedMessages"_s;
    case RestApiUtil::RestApiUrlType::ModerationReports:
        return u"moderation.reports"_s;
    case RestApiUtil::RestApiUrlType::ModerationReportInfo:
        return u"moderation.reportInfo"_s;
    case RestApiUtil::RestApiUrlType::ModerationUserReports:
        return u"moderation.userReports"_s;
    case RestApiUtil::RestApiUrlType::ModerationUserReportsByUserId:
        return u"moderation.user.reportsByUserId"_s;
    case RestApiUtil::RestApiUrlType::ModerationDismissUserReports:
        return u"moderation.dismissUserReports"_s;
    case RestApiUtil::RestApiUrlType::ModerationReportUser:
        return u"moderation.reportUser"_s;

    case RestApiUtil::RestApiUrlType::RoomsNameExists:
        return u"rooms.nameExists"_s;
    case RestApiUtil::RestApiUrlType::MethodCall:
        return u"method.call"_s;
    case RestApiUtil::RestApiUrlType::MethodCallAnon:
        return u"method.callAnon"_s;
    case RestApiUtil::RestApiUrlType::AppsUiInteraction:
        return u"ui.interaction"_s;

    case RestApiUtil::RestApiUrlType::UsersLogoutOtherClients:
        return u"users.logoutOtherClients"_s;

    // 6.8.0
    case RestApiUtil::RestApiUrlType::RoomsMuteUser:
        return u"rooms.muteUser"_s;
    case RestApiUtil::RestApiUrlType::RoomsUnmuteUser:
        return u"rooms.unmuteUser"_s;

    case RestApiUtil::RestApiUrlType::FeaturedApps:
        return u"featured-apps"_s;
    case RestApiUtil::RestApiUrlType::CategoriesApps:
        return u"categories"_s;
    case RestApiUtil::RestApiUrlType::CountApps:
        return u"count"_s;
    case RestApiUtil::RestApiUrlType::NotifyAdminsApps:
        return u"notify-admins"_s;
    case RestApiUtil::RestApiUrlType::InstalledApps:
        return u"installed"_s;
    case RestApiUtil::RestApiUrlType::MarketplaceApps:
        return u"marketplace"_s;

    case RestApiUtil::RestApiUrlType::UsersListByStatus:
        return u"users.listByStatus"_s;

    case RestApiUtil::RestApiUrlType::UsersSendWelcomeEmail:
        return u"users.sendWelcomeEmail"_s;
    case RestApiUtil::RestApiUrlType::Empty:
        return {};
    case RestApiUtil::RestApiUrlType::RoomsImages:
        return u"rooms.images"_s;
    case RestApiUtil::RestApiUrlType::RoomsMembersOrderedByRole:
        return u"rooms.membersOrderedByRole"_s;
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
        return u"/api/v1/"_s;
    case RestApiUrlExtensionType::Apps:
        return u"/api/apps/"_s;
    case RestApiUtil::RestApiUrlExtensionType::NoExtension:
        return u"/api/"_s;
    }
    return {};
}
