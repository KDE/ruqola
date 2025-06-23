/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "systemmessagetypeutil.h"
#include "ruqola_debug.h"

using namespace SystemMessageTypeUtil;
using namespace Qt::Literals::StringLiterals;

SystemMessageType SystemMessageTypeUtil::systemMessageTypeFromString(const QString &str)
{
    if (str == "uj"_L1) {
        return UserJoined;
    } else if (str == "ul"_L1) {
        return UserLeft;
    } else if (str == "ult"_L1) {
        return UserLeftTeam;
    } else if (str == "room_changed_topic"_L1) {
        return RoomTopicChanged;
    } else if (str == "au"_L1) {
        return UserAdded;
    } else if (str == u'r') {
        return RoomNameChanged;
    } else if (str == "ru"_L1) {
        return UserRemoved;
    } else if (str == "room_changed_description"_L1) {
        return RoomDescriptionChanged;
    } else if (str == "room_changed_announcement"_L1) {
        return RoomAnnoucementChanged;
    } else if (str == "room_changed_privacy"_L1) {
        return RoomPrivacyChanged;
    } else if (str == "jitsi_call_started"_L1) {
        return JitsiCallStarted;
    } else if (str == "rm"_L1) {
        return MessageDeleted;
    } else if (str == "message_pinned"_L1) {
        return Pinned;
    } else if (str == "otr"_L1) {
        return EncryptedMessage;
    } else if (str == "user-unmuted"_L1) {
        return UserUnmuted;
    } else if (str == "user-muted"_L1) {
        return UserMuted;
    } else if (str == "subscription-role-added"_L1) {
        return SubscriptionRoleAdded;
    } else if (str == "subscription-role-removed"_L1) {
        return SubscriptionRoleRemoved;
    } else if (str == "e2e"_L1) {
        return MessageE2E;
    } else if (str == "discussion-created"_L1) {
        return DiscussionCreated;
    } else if (str == "ut"_L1) {
        return UserJoinedConversation;
    } else if (str == "room-archived"_L1) {
        return RoomArchived;
    } else if (str == "room-unarchived"_L1) {
        return RoomUnarchived;
    } else if (str == "rtc"_L1) {
        return Rtc;
    } else if (str == "wm"_L1) {
        return Welcome;
    } else if (str == "room_changed_avatar"_L1) {
        return RoomAvatarChanged;
    } else if (str == "room_e2e_enabled"_L1) {
        return RoomE2eEnabled;
    } else if (str == "room_e2e_disabled"_L1) {
        return RoomE2eDisabled;
    } else if (str == "room-set-read-only"_L1) {
        return RoomSetReadOnly;
    } else if (str == "room-removed-read-only"_L1) {
        return RoomRemoveReadOnly;
    } else if (str == "added-user-to-team"_L1) {
        return AddedUserToTeam;
    } else if (str == "removed-user-from-team"_L1) {
        return RemovedUserFromTeam;
    } else if (str == "user-converted-to-team"_L1) {
        return UserConvertedToTeam;
    } else if (str == "user-converted-to-channel"_L1) {
        return UserConvertedToChannel;
    } else if (str == "user-removed-room-from-team"_L1) {
        return UserRemovedRoomFromTeam;
    } else if (str == "user-deleted-room-from-team"_L1) {
        return UserDeletedRoomFromTeam;
    } else if (str == "user-added-room-to-team"_L1) {
        return UserAddedRoomToTeam;
    } else if (str == "room-allowed-reacting"_L1) {
        return RoomAllowedReacting;
    } else if (str == "room-disallowed-reacting"_L1) {
        return RoomDisallowedReacting;
    } else if (str == "ujt"_L1) {
        return UserJoinedTeam;
    } else if (str == "user_joined_otr"_L1) {
        return UserJoinedOtr;
    } else if (str == "user_key_refreshed_successfully"_L1) {
        return UserKeyRefreshedSuccessfully;
    } else if (str == "user_requested_otr_key_refresh"_L1) {
        return UserRequesterOtrKeyRefresh;
    } else if (str == "videoconf"_L1) {
        return VideoConf;
    } else {
        qCWarning(RUQOLA_LOG) << " Unknown message type " << str;
        return Unknown;
    }
}

QString SystemMessageTypeUtil::systemMessageTypeStringFromEnum(SystemMessageType type)
{
    switch (type) {
    case UserJoined:
        return "uj"_L1;
    case UserLeft:
        return "ul"_L1;
    case UserLeftTeam:
        return "ult"_L1;
    case RoomTopicChanged:
        return "room_changed_topic"_L1;
    case UserAdded:
        return "au"_L1;
    case RoomNameChanged:
        return "r"_L1;
    case UserRemoved:
        return "ru"_L1;
    case RoomDescriptionChanged:
        return "room_changed_description"_L1;
    case RoomAnnoucementChanged:
        return "room_changed_announcement"_L1;
    case RoomPrivacyChanged:
        return "room_changed_privacy"_L1;
    case JitsiCallStarted:
        return "jitsi_call_started"_L1;
    case MessageDeleted:
        return "rm"_L1;
    case Pinned:
        return "message_pinned"_L1;
    case EncryptedMessage:
        return "otr"_L1;
    case UserUnmuted:
        return "user-unmuted"_L1;
    case UserMuted:
        return "user-muted"_L1;
    case SubscriptionRoleAdded:
        return "subscription-role-added"_L1;
    case SubscriptionRoleRemoved:
        return "subscription-role-removed"_L1;
    case MessageE2E:
        return "e2e"_L1;
    case DiscussionCreated:
        return "discussion-created"_L1;
    case UserJoinedConversation:
        return "ut"_L1;
    case RoomArchived:
        return "room-archived"_L1;
    case RoomUnarchived:
        return "room-unarchived"_L1;
    case Rtc:
        return "rtc"_L1;
    case Welcome:
        return "wm"_L1;
    case RoomAvatarChanged:
        return "room_changed_avatar"_L1;
    case RoomE2eEnabled:
        return "room_e2e_enabled"_L1;
    case RoomE2eDisabled:
        return "room_e2e_disabled"_L1;
    case RoomSetReadOnly:
        return "room-set-read-only"_L1;
    case RoomRemoveReadOnly:
        return "room-removed-read-only"_L1;
    case AddedUserToTeam:
        return "added-user-to-team"_L1;
    case RemovedUserFromTeam:
        return "removed-user-from-team"_L1;
    case UserConvertedToTeam:
        return "user-converted-to-team"_L1;
    case UserConvertedToChannel:
        return "user-converted-to-channel"_L1;
    case UserRemovedRoomFromTeam:
        return "user-removed-room-from-team"_L1;
    case UserDeletedRoomFromTeam:
        return "user-deleted-room-from-team"_L1;
    case UserAddedRoomToTeam:
        return "user-added-room-to-team"_L1;
    case RoomAllowedReacting:
        return "room-allowed-reacting"_L1;
    case RoomDisallowedReacting:
        return "room-disallowed-reacting"_L1;
    case UserJoinedTeam:
        return "ujt"_L1;
    case UserJoinedOtr:
        return "user_joined_otr"_L1;
    case UserKeyRefreshedSuccessfully:
        return "user_key_refreshed_successfully"_L1;
    case UserRequesterOtrKeyRefresh:
        return "user_requested_otr_key_refresh"_L1;
    case VideoConf:
        return "videoconf"_L1;
    case Unknown:
        return {};
    }
    return {};
}
