/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QString>

namespace SystemMessageTypeUtil
{
enum SystemMessageType {
    Unknown = 0,
    UserJoined,
    UserLeft,
    UserLeftTeam,
    RoomTopicChanged,
    UserAdded,
    RoomNameChanged,
    UserRemoved,
    RoomDescriptionChanged,
    RoomAnnoucementChanged,
    RoomPrivacyChanged,
    JitsiCallStarted,
    MessageDeleted,
    Pinned,
    EncryptedMessage,
    UserMuted,
    UserUnmuted,
    SubscriptionRoleAdded,
    SubscriptionRoleRemoved,
    MessageE2E,
    DiscussionCreated,
    UserJoinedConversation,
    RoomArchived,
    RoomUnarchived,
    Rtc,
    Welcome,
    RoomAvatarChanged,
    RoomE2eEnabled,
    RoomE2eDisabled,
    RoomSetReadOnly,
    RoomRemoveReadOnly,
    AddedUserToTeam,
    RemovedUserFromTeam,
    UserConvertedToTeam,
    UserConvertedToChannel,
    UserRemovedRoomFromTeam,
    UserDeletedRoomFromTeam,
    UserAddedRoomToTeam,
    RoomAllowedReacting,
    RoomDisallowedReacting,
    UserJoinedTeam,
    UserJoinedOtr,
    UserKeyRefreshedSuccessfully,
    UserRequesterOtrKeyRefresh,
    VideoConf,
    LastElement = VideoConf,
};

[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT SystemMessageType systemMessageTypeFromString(const QString &str);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QString systemMessageTypeStringFromEnum(SystemMessageType type);
}
