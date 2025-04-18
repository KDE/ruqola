/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include <QString>

namespace RestApiUtil
{

enum class RestApiUrlExtensionType : uint8_t {
    NoExtension,
    V1,
    Apps,
};

enum class RestApiUrlType {
    Login,
    Logout,
    Me,
    UsersCreate,
    UsersDelete,
    UsersCreateToken,
    UsersGetPresence,
    UsersInfo,
    UsersList,
    UsersRegister,
    UsersGetAvatar,
    UsersResetAvatar,
    UsersSetAvatar,
    UsersUpdate,
    UsersPresence, /*since 1.1.0*/
    UsersUpdateOwnBasicInfo,

    // PersonalAccess Token
    UsersGeneratePersonalAccessToken,
    UsersRegeneratePersonalAccessToken,
    UsersRemovePersonalAccessToken,
    UsersGetPersonalAccessTokens,

    UsersGetUsernameSuggestion,
    UsersSetStatus,
    UsersGetStatus,
    /* ... 2.4 ? */
    UsersAutocomplete,
    /* ... 3.1 ? */
    UsersRemoveOtherTokens,
    UsersDeleteOwnAccount,
    UsersSetActiveStatus,

    UsersResetTOTP,
    UsersResetE2EKey,

    ChatDelete,
    ChatGetMessage,
    ChatPinMessage,
    ChatPostMessage,
    ChatReact,
    ChatSearch,
    ChatStarMessage,
    ChatUnPinMessage,
    ChatUnStarMessage,
    ChatUpdate,
    ChatIgnoreUser,
    ChatReportMessage,
    // Since 1.0.0
    ChatFollowMessage,
    ChatUnFollowMessage,
    ChatGetThreadsList,
    ChatSyncThreadMessages,
    ChatGetThreadMessages,
    ChatSyncThreadsList,
    ChatGetDeletedMessage,
    ChatSendMessage,
    ChatMessageReadReceipts,
    // Since 2.0.0
    ChatGetPinnedMessages,
    ChatGetMentionedMessages,

    // Since 2.2.x
    ChatGetStarredMessages,
    ChatGetSnippetedMessages,

    ChatSyncMessages,

    ChannelsAddAll,
    //@since 0.75
    ChannelsAddLeader,
    ChannelsAddModerator,
    ChannelsAddOwner,
    ChannelsArchive,
    ChannelsClose,
    ChannelsCreate,
    ChannelsGetIntegrations,
    ChannelsHistory,
    ChannelsInfo,
    ChannelsInvite,
    ChannelsKick,
    ChannelsLeave,
    ChannelsList,
    ChannelsListJoined,
    ChannelsOpen,
    ChannelsRemoveModerator,
    ChannelsRemoveOwner,
    //@since 0.75
    ChannelsRemoveLeader,
    ChannelsRename,
    ChannelsSetDescription,
    ChannelsSetJoinCode,
    ChannelsSetPurpose,
    ChannelsSetReadOnly,
    ChannelsSetTopic,
    ChannelsSetType,
    ChannelsSetAnnouncement,
    ChannelsFiles,
    ChannelsUnarchive,
    ChannelsCounters,
    ChannelsMembers,
    ChannelsJoin,
    ChannelsOnline,
    // since 0.70
    ChannelsModerators,
    // Since 0.71
    ChannelsDelete,
    // since 0.65
    ChannelsRoles,
    // since 0.63
    ChannelsGetAllUserMentionsByChannel,
    GroupsAddAll,
    GroupsAddModerator,
    GroupsAddOwner,
    GroupsAddLeader,
    GroupsArchive,
    GroupsClose,
    GroupsCreate,
    // since 0.70
    GroupsDelete,
    GroupsGetIntegrations,
    GroupsHistory,
    GroupsInfo,
    GroupsInvite,
    GroupsKick,
    GroupsLeave,
    GroupsList,
    GroupsOpen,
    GroupsRemoveModerator,
    GroupsRemoveOwner,
    GroupsRemoveLeader,
    GroupsRename,
    GroupsSetDescription,
    GroupsSetPurpose,
    GroupsSetReadOnly,
    GroupsSetTopic,
    GroupsSetType,
    GroupsUnarchive,
    GroupsSetAnnouncement,
    GroupsFiles,
    GroupsListAll,
    GroupsMembers,
    // Since 0.65
    GroupsRoles,
    GroupsCounters,
    GroupsSetEncrypted,
    ServerInfo,
    Settings,
    SettingsPublic,
    UpdateAdminSettings,
    Spotlight,
    LoadEmojiCustom,
    EmojiCustomDelete,
    EmojiCustomCreate,
    EmojiCustomUpdate,
    EmojiCustomAll,

    RoomsUpload,
    RoomsSaveNotification,
    RoomsSaveSettings,
    RoomsAdminRooms,
    RoomsAdminRoomsGetRoom,
    ImClose,
    ImCreate,
    ImHistory,
    ImFiles,
    ImOpen,
    ImMessages,
    ImDelete,
    // Since 0.59
    ImMembers,
    ImSetTopic,
    SubscriptionsRead,
    SubscriptionsUnRead,
    SettingsOauth,
    SettingsAddCustomOauth,
    RoomsGet,
    RoomsFavorite,
    RoomsCleanHistory,
    RoomsInfo,
    RoomsLeave,
    RoomsCreateDiscussion,
    RoomsGetDiscussions,
    RoomsExport,
    RoomsChangeArchivactionState,
    RoomsNameExists,

    ForgotPassword,
    CommandsList,
    CommandsGet,
    CommandsRun,
    CommandsPreview,
    // since 0.70
    E2ESetRoomKeyID,
    E2EfetchMyKeys,
    E2EupdateGroupKey,
    E2ESetUserPublicAndPrivateKeys,
    E2EGetUsersOfRoomWithoutKey,
    E2ERequestSubscriptionKeys,
    E2EResetOwnE2EKey,

    E2EAcceptSuggestedGroupKey,
    E2ERejectSuggestedGroupKey,
    E2EProvideUsersWithSuggestedGroupKeys,
    E2EResetRoomKey,
    E2EFetchUsersWaitingForGroupKey,

    RolesList,
    RolesCreate,
    RolesUpdate,
    RolesSync,
    RolesDelete,
    RolesAddUserToRole,
    RolesGetUsersInRole,
    RolesRemoveUserFromRole,

    // since 0.74
    VideoConfJitsi,

    // Autotranslate @since 2.0
    AutoTranslateGetSupportedLanguages,
    AutoTranslateSaveSettings,
    AutoTranslateTranslateMessage,

    // Since 2.4
    CustomUserStatusList,
    CustomUserStatusCreate,
    CustomUserStatusDelete,
    CustomUserStatusUpdate,
    CustomSoundsList,

    // Invite since 2.5 ?
    FindOrCreateInvite,
    ListInvites,
    RemoveInvite,
    UseInviteToken,
    ValidateInviteToken,

    // RC 6.0
    SendInvitationEmails,

    // Since 2.4.0
    RoomsAutocompleteChannelAndPrivate,

    // two factor authentication
    Users2FASendEmailCode,
    Users2FAEnableEmail,
    Users2FADisableEmail,

    UsersSetPreferences,

    // Permission
    PermissionsListAll,
    PermissionsUpdate,

    Statistics,

    Directory,

    // Teams 3.13
    TeamsList,
    TeamsListAll,
    TeamsCreate,
    TeamsAddRooms,
    TeamsRemoveRoom,
    TeamsUpdateRoom,
    TeamsListRooms,
    TeamsListRoomsOfUser,
    TeamsMembers,
    TeamsAddMembers,
    TeamsUpdateMember,
    TeamsRemoveMember,
    TeamsLeave,
    TeamsInfo,
    TeamsDelete,
    TeamsAutocomplete,
    TeamsConvertToChannel,
    RoomsAutocompleteAvailableForTeams,
    ChannelsConvertToTeam,
    GroupsConvertToTeam,

    StdoutQueue,

    // Oauth
    OauthAppsList,
    OauthAppsGet,
    // 5.4.0
    OauthAppsCreate,
    OauthAppsUpdate,
    OauthAppsDelete,

    // License
    LicensesMaxActiveUsers,
    LicensesAdd,
    LicensesIsEntreprise,
    // 6.5.0
    LicensesInfo,

    // Banner
    BannersDismiss,
    Banners,

    // Session
    SessionsList,
    SessionsInfo,
    SessionsListAll,
    SessionsLogoutMe,
    SessionsLogout,
    SessionsInfoAdmin,

    UserRequestDataDownload,

    // 5.4.0
    RoomsDelete,

    // Video Conference
    VideoConferenceStart,
    VideoConferenceJoin,
    VideoConferenceCancel,
    VideoConferenceInfo,
    VideoConferenceList,
    VideoConferenceProviders,
    VideoConferenceCapabilities,

    // Moderation
    ModerationReportsByUsers,
    ModerationDismissReports,
    ModerationUserReportedMessages,
    ModerationUserDeleteReportedMessages,
    ModerationReports,
    ModerationReportInfo,
    ModerationUserReports,
    ModerationUserReportsByUserId,
    ModerationDismissUserReports,
    ModerationReportUser,

    // Proxy
    MethodCall,
    MethodCallAnon,

    // Apps.ui.interaction
    AppsUiInteraction,

    // users.logoutOtherClients
    UsersLogoutOtherClients,

    // rooms.muteUser
    RoomsMuteUser,
    RoomsUnmuteUser,

    // /api/apps/ (applications)
    FeaturedApps,
    CategoriesApps,
    CountApps,
    NotifyAdminsApps,
    InstalledApps,
    MarketplaceApps,

    // users.listByStatus
    UsersListByStatus,

    // users.sendWelcomeEmail
    UsersSendWelcomeEmail,

    // Used by apps info => we need empty string here.
    Empty,

    // rooms.images
    RoomsImages,

    // rooms.membersOrderedByRole (7.3.0)
    RoomsMembersOrderedByRole,
};
LIBROCKETCHATRESTAPI_QT_EXPORT QString adaptUrl(const QString &url);
LIBROCKETCHATRESTAPI_QT_EXPORT QString restUrl(RestApiUtil::RestApiUrlType type);
LIBROCKETCHATRESTAPI_QT_EXPORT QByteArray convertSha256Password(const QString &pwd);
LIBROCKETCHATRESTAPI_QT_EXPORT QString restApiUrlExtensionType(RestApiUtil::RestApiUrlExtensionType type);
}
