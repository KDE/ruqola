/*
   Copyright (c) 2017-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#pragma once

#include "librestapi_private_export.h"
#include <QString>

namespace RestApiUtil
{
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
    UsersRegeneratePersonalAccessToken,
    UsersGetUsernameSuggestion,
    UsersRemovePersonalAccessToken,
    UsersGeneratePersonalAccessToken,
    UsersSetStatus,
    UsersGetStatus,
    /* ... 2.4 ? */
    UsersAutocomplete,
    /* ... 3.1 ? */
    UsersRemoveOtherTokens,
    UsersDeleteOwnAccount,

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

    // Since 2.2.x
    ChatGetStarredMessages,
    ChatGetSnippetedMessages,

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
    GroupRemoveOwner,
    GroupRemoveLeader,
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
    Spotlight,
    LoadEmojiCustom,
    EmojiCustomDelete,
    EmojiCustomCreate,
    EmojiCustomUpdate,
    RoomsUpload,
    RoomsSaveNotification,
    RoomsSaveSettings,
    RoomsAdminRooms,
    ImClose,
    ImCreate,
    ImHistory,
    ImFiles,
    ImOpen,
    ImMessages,
    // Since 0.59
    ImMembers,
    ImSetTopic,
    SubscriptionsRead,
    SubscriptionsUnRead,
    SettingsOauth,
    RoomsGet,
    RoomsFavorite,
    RoomsCleanHistory,
    RoomsInfo,
    RoomsLeave,
    RoomsCreateDiscussion,
    RoomsGetDiscussions,
    RoomsExport,

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

    RolesList,
    RolesCreate,
    RolesSync,
    RolesAddUserToRole,
    RolesGetUsersInRole,
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
};
LIBROCKETCHATRESTAPI_QT5_EXPORT QString adaptUrl(const QString &url);
LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT QString restUrl(RestApiUtil::RestApiUrlType type);
LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT QString apiUri();
LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT QByteArray convertSha256Password(const QString &pwd);
}

