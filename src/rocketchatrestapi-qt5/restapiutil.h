/*
   Copyright (c) 2017-2019 Montel Laurent <montel@kde.org>

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

#ifndef RESTAPIUTIL_H
#define RESTAPIUTIL_H

#include <QString>
#include "librestapi_private_export.h"

namespace RestApiUtil {
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
    UsersGetUsernameSuggestion,
    UsersRemovePersonalAccessToken,
    UsersGeneratePersonalAccessToken,
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
    ChannelsAddAll,
    ChannelsAddModerator,
    ChannelsAddOwner,
    ChannelsArchive,
    ChannelsCleanHistory,
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
    //since 0.70
    ChannelsModerators,
    //Since 0.71
    ChannelsDelete,
    //since 0.65
    ChannelsRoles,
    //since 0.63
    ChannelsGetAllUserMentionsByChannel,
    ChannelsSetEncrypted,
    GroupsAddAll,
    GroupsAddModerator,
    GroupsAddOwner,
    GroupsAddLeader,
    GroupsArchive,
    GroupsClose,
    GroupsCreate,
    //since 0.70
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
    //Since 0.65
    GroupsRoles,
    GroupsCounters,
    GroupsSetEncrypted,
    ServerInfo,
    Settings,
    SettingsPublic,
    Spotlight,
    LoadEmojiCustom,
    RoomsUpload,
    RoomsSaveNotification,
    ImClose,
    ImCreate,
    ImHistory,
    ImFiles,
    ImOpen,
    ImMessages,
    //Since 0.59
    ImMembers,
    ImSetTopic,
    SubscriptionsRead,
    SubscriptionsUnRead,
    SettingsOauth,
    RoomsGet,
    RoomsFavorite,
    RoomscleanHistory,
    RoomsInfo,
    RoomsLeave,
    ForgotPassword,
    PermissionsList,
    CommandsList,
    //since 0.70
    E2ESetRoomKeyID,
    E2EfetchMyKeys,
    E2EupdateGroupKey,
    E2ESetUserPublicAndPrivateKeys,
    E2EGetUsersOfRoomWithoutKey,
    E2ERequestSubscriptionKeys,

    RolesList,
    RolesCreate,
    RolesAddUserToRole,
    //since 0.74
    VideoConfJitsi,
};
LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT QString adaptUrl(const QString &url);
LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT QString restUrl(RestApiUtil::RestApiUrlType type);
LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT QString apiUri();
}

#endif // RESTAPIUTIL_H
