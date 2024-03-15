/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "systemmessagetypeutil.h"

using namespace SystemMessageTypeUtil;

SystemMessageType SystemMessageTypeUtil::systemMessageTypeFromString(const QString &str)
{
    if (str == QLatin1StringView("uj")) {
        return UserJoined;
    } else if (str == QLatin1StringView("ul")) {
        return UserLeft;
    } else if (str == QLatin1StringView("ult")) {
        return UserLeftChannel;
    } else if (str == QLatin1StringView("room_changed_topic")) {
        return RoomTopicChanged;
    } else if (str == QLatin1StringView("au")) {
        return UserAdded;
    } else if (str == QLatin1Char('r')) {
        return RoomNameChanged;
    } else if (str == QLatin1StringView("ru")) {
        return UserRemoved;
    } else if (str == QLatin1StringView("room_changed_description")) {
        return RoomDescriptionChanged;
    } else if (str == QLatin1StringView("room_changed_announcement")) {
        return RoomAnnoucementChanged;
    } else if (str == QLatin1StringView("room_changed_privacy")) {
        return RoomPrivacyChanged;
    } else if (str == QLatin1StringView("jitsi_call_started")) {
        return JitsiCallStarted;
    } else if (str == QLatin1StringView("rm")) {
        return MessageDeleted;
    } else if (str == QLatin1StringView("message_pinned")) {
        return MessagePinned;
    } else if (str == QLatin1StringView("otr")) {
        return EncryptedMessage;
    } else if (str == QLatin1StringView("user-unmuted")) {
        return UserUnmuted;
    } else if (str == QLatin1StringView("user-muted")) {
        return UserMuted;
    } else if (str == QLatin1StringView("subscription-role-added")) {
        return SubscriptionRoleAdded;
    } else if (str == QLatin1StringView("subscription-role-removed")) {
        return SubscriptionRoleRemoved;
    } else if (str == QLatin1StringView("e2e")) {
        return MessageE2E;
    } else if (str == QLatin1StringView("discussion-created")) {
        return DiscussionCreated;
    } else if (str == QLatin1StringView("ut")) {
        return UserJoinedConversation;
    } else if (str == QLatin1StringView("room-archived")) {
        return RoomArchived;
    } else if (str == QLatin1StringView("room-unarchived")) {
        return RoomUnarchived;
    } else if (str == QLatin1StringView("rtc")) {
        return Rtc;
    } else if (str == QLatin1StringView("wm")) {
        return Welcome;
    } else if (str == QLatin1StringView("room_changed_avatar")) {
        return RoomAvatarChanged;
    } else if (str == QLatin1StringView("room_e2e_enabled")) {
        return RoomE2eEnabled;
    } else if (str == QLatin1StringView("room_e2e_disabled")) {
        return RoomE2eDisabled;
    } else if (str == QLatin1StringView("room-set-read-only")) {
        return RoomSetReadOnly;
    } else if (str == QLatin1StringView("room-removed-read-only")) {
        return RoomRemoveReadOnly;
    } else if (str == QLatin1StringView("added-user-to-team")) {
        return AddedUserToTeam;
    } else if (str == QLatin1StringView("removed-user-from-team")) {
        return RemovedUserFromTeam;
    } else if (str == QLatin1StringView("user-converted-to-team")) {
        return UserConvertedToTeam;
    } else if (str == QLatin1StringView("user-converted-to-channel")) {
        return UserConvertedToChannel;
    } else if (str == QLatin1StringView("user-removed-room-from-team")) {
        return UserRemovedRoomFromTeam;
    } else if (str == QLatin1StringView("user-deleted-room-from-team")) {
        return UserDeletedRoomFromTeam;
    } else if (str == QLatin1StringView("user-added-room-to-team")) {
        return UserAddedRoomToTeam;
    } else if (str == QLatin1StringView("room-allowed-reacting")) {
        return RoomAllowedReacting;
    } else if (str == QLatin1StringView("room-disallowed-reacting")) {
        return RoomDisallowedReacting;
    } else if (str == QLatin1StringView("ujt")) {
        return UserJoinedTeam;
    } else if (str == QLatin1StringView("user_joined_otr")) {
        return UserJoinedOtr;
    } else if (str == QLatin1StringView("user_key_refreshed_successfully")) {
        return UserKeyRefreshedSuccessfully;
    } else if (str == QLatin1StringView("user_requested_otr_key_refresh")) {
        return UserRequesterOtrKeyRefresh;
    } else if (str == QLatin1StringView("videoconf")) {
        return VideoConf;
    } else {
        return Unknown;
    }
}

QString SystemMessageTypeUtil::systemMessageTypeStringFromEnum(SystemMessageType type)
{
    switch (type) {
    case UserJoined:
        return QLatin1StringView("uj");
    case UserLeft:
        return QLatin1StringView("ul");
    case UserLeftChannel:
        return QLatin1StringView("ult");
    case RoomTopicChanged:
        return QLatin1StringView("room_changed_topic");
    case UserAdded:
        return QLatin1StringView("au");
    case RoomNameChanged:
        return QLatin1StringView("r");
    case UserRemoved:
        return QLatin1StringView("ru");
    case RoomDescriptionChanged:
        return QLatin1StringView("room_changed_description");
    case RoomAnnoucementChanged:
        return QLatin1StringView("room_changed_announcement");
    case RoomPrivacyChanged:
        return QLatin1StringView("room_changed_privacy");
    case JitsiCallStarted:
        return QLatin1StringView("jitsi_call_started");
    case MessageDeleted:
        return QLatin1StringView("rm");
    case MessagePinned:
        return QLatin1StringView("message_pinned");
    case EncryptedMessage:
        return QLatin1StringView("otr");
    case UserUnmuted:
        return QLatin1StringView("user-unmuted");
    case UserMuted:
        return QLatin1StringView("user-muted");
    case SubscriptionRoleAdded:
        return QLatin1StringView("subscription-role-added");
    case SubscriptionRoleRemoved:
        return QLatin1StringView("subscription-role-removed");
    case MessageE2E:
        return QLatin1StringView("e2e");
    case DiscussionCreated:
        return QLatin1StringView("discussion-created");
    case UserJoinedConversation:
        return QLatin1StringView("ut");
    case RoomArchived:
        return QLatin1StringView("room-archived");
    case RoomUnarchived:
        return QLatin1StringView("room-unarchived");
    case Rtc:
        return QLatin1StringView("rtc");
    case Welcome:
        return QLatin1StringView("wm");
    case RoomAvatarChanged:
        return QLatin1StringView("room_changed_avatar");
    case RoomE2eEnabled:
        return QLatin1StringView("room_e2e_enabled");
    case RoomE2eDisabled:
        return QLatin1StringView("room_e2e_disabled");
    case RoomSetReadOnly:
        return QLatin1StringView("room-set-read-only");
    case RoomRemoveReadOnly:
        return QLatin1StringView("room-removed-read-only");
    case AddedUserToTeam:
        return QLatin1StringView("added-user-to-team");
    case RemovedUserFromTeam:
        return QLatin1StringView("removed-user-from-team");
    case UserConvertedToTeam:
        return QLatin1StringView("user-converted-to-team");
    case UserConvertedToChannel:
        return QLatin1StringView("user-converted-to-channel");
    case UserRemovedRoomFromTeam:
        return QLatin1StringView("user-removed-room-from-team");
    case UserDeletedRoomFromTeam:
        return QLatin1StringView("user-deleted-room-from-team");
    case UserAddedRoomToTeam:
        return QLatin1StringView("user-added-room-to-team");
    case RoomAllowedReacting:
        return QLatin1StringView("room-allowed-reacting");
    case RoomDisallowedReacting:
        return QLatin1StringView("room-disallowed-reacting");
    case UserJoinedTeam:
        return QLatin1StringView("ujt");
    case UserJoinedOtr:
        return QLatin1StringView("user_joined_otr");
    case UserKeyRefreshedSuccessfully:
        return QLatin1StringView("user_key_refreshed_successfully");
    case UserRequesterOtrKeyRefresh:
        return QLatin1StringView("user_requested_otr_key_refresh");
    case VideoConf:
        return QLatin1StringView("videoconf");
    case Unknown:
        return {};
    }
    return {};
}
