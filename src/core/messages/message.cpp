/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "message.h"
#include "ruqola_debug.h"
#include <KLocalizedString>
#include <QCborValue>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
Message::Message() = default;

Message::~Message() = default;

void Message::parseMessage(const QJsonObject &o, bool restApi, EmojiManager *emojiManager)
{
    mMessageId = o.value("_id"_L1).toString().toLatin1();
    mRoomId = o.value("rid"_L1).toString().toLatin1();
    mText = o.value("msg"_L1).toString();
    if (restApi) {
        mUpdatedAt = Utils::parseIsoDate(u"_updatedAt"_s, o);
        setEditedAt(Utils::parseIsoDate(u"editedAt"_s, o));
        setTimeStamp(Utils::parseIsoDate(u"ts"_s, o));
        if (o.contains(u"tlm"_s)) {
            setThreadLastMessage(Utils::parseIsoDate(u"tlm"_s, o));
        }
        if (o.contains(u"dlm"_s)) {
            setDiscussionLastMessage(Utils::parseIsoDate(u"dlm"_s, o));
        }
    } else {
        setTimeStamp(Utils::parseDate(u"ts"_s, o));
        mUpdatedAt = Utils::parseDate(u"_updatedAt"_s, o);
        setEditedAt(Utils::parseDate(u"editedAt"_s, o));
        // Verify if a day we will use not restapi for it.
        if (o.contains(u"tlm"_s)) {
            setThreadLastMessage(Utils::parseDate(u"tlm"_s, o));
        }
        // Verify if a day we will use not restapi for it.
        if (o.contains(u"dlm"_s)) {
            setDiscussionLastMessage(Utils::parseDate(u"dlm"_s, o));
        }
    }

    const auto userObject = o.value("u"_L1).toObject();
    mUsername = userObject.value("username"_L1).toString();
    mName = userObject.value("name"_L1).toString();
    mUserId = userObject.value("_id"_L1).toString().toLatin1();
    mEditedByUsername = o.value("editedBy"_L1).toObject().value("username"_L1).toString();
    mAlias = o.value("alias"_L1).toString();
    mAvatar = o.value("avatar"_L1).toString();

    setUnread(o.value("unread"_L1).toBool());
    setGroupable(o.value("groupable"_L1).toBool(/*true*/ false)); // Laurent, disable for the moment groupable
    setParseUrls(o.value("parseUrls"_L1).toBool(false));
    mRole = o.value("role"_L1).toString();
    if (o.contains("tcount"_L1)) {
        setThreadCount(o.value("tcount"_L1).toInt());
    }
    if (o.contains("dcount"_L1)) {
        setDiscussionCount(o.value("dcount"_L1).toInt());
    }
    if (o.contains("drid"_L1)) {
        setDiscussionRoomId(o.value("drid"_L1).toString().toLatin1());
    }
    if (o.contains("tmid"_L1)) {
        setThreadMessageId(o.value("tmid"_L1).toString().toLatin1());
    }
    mEmoji = o.value("emoji"_L1).toString();
    mMessageStarred.parse(o);

    if (o.contains("pinned"_L1)) {
        MessagePinned pinned;
        pinned.parse(o);
        if (pinned.isValid()) {
            setMessagePinned(pinned);
        }
    }

    // Translation
    if (o.contains("translations"_L1)) {
        MessageTranslations translation;
        translation.parse(o);
        if (!translation.isEmpty()) {
            setMessageTranslation(translation);
        }
    } else {
        mMessageTranslation.reset();
    }
    setPrivateMessage(o.value("private"_L1).toBool(false));

    const QString type = o.value("t"_L1).toString();
    if (!type.isEmpty()) {
        if (type == "videoconf"_L1) {
            mMessageType = MessageType::VideoConference;
            // qDebug() << " VIDEO " << o;
        } else if (type == "e2e"_L1) {
            mSystemMessageType = SystemMessageTypeUtil::systemMessageTypeFromString(type);
            mMessageType = MessageType::EncryptedText;
            qDebug() << " encrypted message !!!!" << mText;
        } else {
            mSystemMessageType = SystemMessageTypeUtil::systemMessageTypeFromString(type);
            mMessageType = MessageType::System;
        }
    } else {
        mMessageType = Message::MessageType::NormalText;
    }
    parseBlocks(o.value("blocks"_L1).toArray());
    parseMentions(o.value("mentions"_L1).toArray());

    parseAttachment(o.value("attachments"_L1).toArray());
    parseMessageUrls(o.value("urls"_L1).toArray());
    parseReactions(o.value("reactions"_L1).toObject(), emojiManager);
    parseChannels(o.value("channels"_L1).toArray());
    parseReplies(o.value("replies"_L1).toArray());
}

void Message::parseReactions(const QJsonObject &reacts, EmojiManager *emojiManager)
{
    if (!reacts.isEmpty()) {
        if (!mReactions) {
            mReactions = new Reactions;
        } else {
            mReactions.reset(new Reactions);
        }
        mReactions->parseReactions(reacts, emojiManager);
    }
}

bool Message::isEditingMode() const
{
    return messageStateValue(Edited);
}

void Message::setIsEditingMode(bool isEditingMode)
{
    assignMessageStateValue(Edited, isEditingMode);
}

bool Message::showIgnoredMessage() const
{
    return messageStateValue(Ignored);
}

void Message::setShowIgnoredMessage(bool showIgnoredMessage)
{
    assignMessageStateValue(Ignored, showIgnoredMessage);
}

bool Message::pendingMessage() const
{
    return messageStateValue(Pending);
}

void Message::setPendingMessage(bool pendingMessage)
{
    assignMessageStateValue(Pending, pendingMessage);
}

QString Message::emoji() const
{
    return mEmoji;
}

void Message::setEmoji(const QString &emoji)
{
    mEmoji = emoji;
}

const Replies *Message::replies() const
{
    if (mReplies) {
        return mReplies.data();
    }
    return nullptr;
}

void Message::setReplies(const Replies &replies)
{
    if (!mReplies) {
        mReplies = new Replies(replies);
    } else {
        mReplies.reset(new Replies(replies));
    }
}

QString Message::name() const
{
    return mName;
}

void Message::setName(const QString &name)
{
    mName = name;
}

bool Message::isAutoTranslated() const
{
    if (mMessageTranslation) {
        return !mMessageTranslation->isEmpty();
    }
    return false;
}

bool Message::isEncryptedMessage() const
{
    // TODO
    return false;
}

bool Message::showTranslatedMessage() const
{
    return messageStateValue(Translated);
}

void Message::setShowTranslatedMessage(bool showOriginalMessage)
{
    assignMessageStateValue(Translated, showOriginalMessage);
}

const MessageTranslations *Message::messageTranslation() const
{
    if (mMessageTranslation) {
        return mMessageTranslation.data();
    }
    return nullptr;
}

void Message::setMessageTranslation(const MessageTranslations &messageTranslation)
{
    if (!mMessageTranslation) {
        mMessageTranslation = new MessageTranslations(messageTranslation);
    } else {
        mMessageTranslation.reset(new MessageTranslations(messageTranslation));
    }
}

QString Message::displayTime() const
{
    return mDisplayTime;
}

QByteArray Message::threadMessageId() const
{
    if (mMessageExtra) {
        return mMessageExtra->threadMessageId();
    }
    return {};
}

void Message::setThreadMessageId(const QByteArray &threadMessageId)
{
    messageExtra()->setThreadMessageId(threadMessageId);
}

QByteArray Message::discussionRoomId() const
{
    if (mMessageExtra) {
        return mMessageExtra->discussionRoomId();
    }
    return {};
}

void Message::setDiscussionRoomId(const QByteArray &discussionRoomId)
{
    messageExtra()->setDiscussionRoomId(discussionRoomId);
}

int Message::discussionCount() const
{
    if (mMessageExtra) {
        return mMessageExtra->discussionCount();
    }
    return {};
}

void Message::setDiscussionCount(int discussionCount)
{
    messageExtra()->setDiscussionCount(discussionCount);
}

qint64 Message::discussionLastMessage() const
{
    if (mMessageExtra) {
        return mMessageExtra->discussionLastMessage();
    }
    return -1;
}

void Message::setDiscussionLastMessage(qint64 discussionLastMessage)
{
    messageExtra()->setDiscussionLastMessage(discussionLastMessage);
}

qint64 Message::threadLastMessage() const
{
    if (mMessageExtra) {
        return mMessageExtra->threadLastMessage();
    }
    return -1;
}

void Message::setThreadLastMessage(qint64 threadLastMessage)
{
    messageExtra()->setThreadLastMessage(threadLastMessage);
}

int Message::threadCount() const
{
    if (mMessageExtra) {
        return mMessageExtra->threadCount();
    }
    return 0;
}

void Message::setThreadCount(int threadCount)
{
    messageExtra()->setThreadCount(threadCount);
}

MessageStarred Message::messageStarred() const
{
    return mMessageStarred;
}

void Message::setMessageStarred(MessageStarred messageStarred)
{
    mMessageStarred = messageStarred;
}

const MessagePinned *Message::messagePinned() const
{
    if (mMessagePinned) {
        return mMessagePinned.data();
    }
    return nullptr;
}

void Message::setMessagePinned(const MessagePinned &messagePinned)
{
    if (!mMessagePinned) {
        mMessagePinned = new MessagePinned(messagePinned);
    } else {
        mMessagePinned.reset(new MessagePinned(messagePinned));
    }
}

bool Message::unread() const
{
    return messageStateValue(Unread);
}

void Message::setUnread(bool unread)
{
    assignMessageStateValue(Unread, unread);
}

QString Message::role() const
{
    return mRole;
}

void Message::setRole(const QString &role)
{
    mRole = role;
}

void Message::parseChannels(const QJsonArray &channels)
{
    if (!channels.isEmpty()) {
        if (!mChannels) {
            mChannels = new Channels;
        } else {
            mChannels.reset(new Channels);
        }
        mChannels->parseChannels(channels);
    } else {
        mChannels.reset();
    }
}

const Blocks *Message::blocks() const
{
    if (mBlocks) {
        return mBlocks.data();
    }
    return nullptr;
}

void Message::setBlocks(const Blocks &newBlocks)
{
    if (!mBlocks) {
        mBlocks = new Blocks(newBlocks);
    } else {
        mBlocks.reset(new Blocks(newBlocks));
    }
}

QString Message::originalMessageOrAttachmentDescription() const
{
    if (!attachments()) {
        return text();
    }
    if (attachments()->isEmpty()) {
        return text();
    }
    return attachments()->messageAttachments().constFirst().description();
}

MessageExtra *Message::messageExtra()
{
    if (!mMessageExtra) {
        mMessageExtra = new MessageExtra;
    }
    return mMessageExtra;
}

int Message::numberOfTextSearched() const
{
    return mNumberOfTextSearched;
}

bool Message::textToSpeechInProgress() const
{
    return messageStateValue(TextToSpeechInProgress);
}

void Message::setTextToSpeechInProgress(bool newTextToSpeechInProgress)
{
    assignMessageStateValue(TextToSpeechInProgress, newTextToSpeechInProgress);
}

QString Message::localTranslation() const
{
    if (!mMessageExtra) {
        return {};
    }
    return mMessageExtra->localTranslation();
}

void Message::setLocalTranslation(const QString &newLocalTranslation)
{
    messageExtra()->setLocalTranslation(newLocalTranslation);
}

bool Message::hoverHighlight() const
{
    return messageStateValue(HoverHighlight);
}

void Message::setHoverHighlight(bool newShowReactionIcon)
{
    assignMessageStateValue(HoverHighlight, newShowReactionIcon);
}

const Channels *Message::channels() const
{
    if (mChannels) {
        return mChannels.data();
    }
    return nullptr;
}

void Message::setChannels(const Channels &channels)
{
    if (!mChannels) {
        mChannels = new Channels(channels);
    } else {
        mChannels.reset(new Channels(channels));
    }
}

void Message::parseReplies(const QJsonArray &replies)
{
    if (!replies.isEmpty()) {
        if (!mReplies) {
            mReplies = new Replies;
        } else {
            mReplies.reset(new Replies);
        }
        mReplies->parseReplies(replies);
    } else {
        mReplies.reset();
    }
}

void Message::parseBlocks(const QJsonArray &blocks)
{
    if (!blocks.isEmpty()) {
        if (!mBlocks) {
            mBlocks = new Blocks;
        } else {
            mBlocks.reset(new Blocks);
        }
        mBlocks->parseBlocks(blocks);
    } else {
        mBlocks.reset();
    }
}

bool Message::privateMessage() const
{
    return messageStateValue(Private);
}

void Message::setPrivateMessage(bool newPrivateMessage)
{
    assignMessageStateValue(Private, newPrivateMessage);
}

const ModerationMessage *Message::moderationMessage() const
{
    if (mModerationMessage) {
        return mModerationMessage.data();
    }
    return nullptr;
}

void Message::setModerationMessage(const ModerationMessage &newModerationMessage)
{
    if (!mModerationMessage) {
        mModerationMessage = new ModerationMessage(newModerationMessage);
    } else {
        mModerationMessage.reset(new ModerationMessage(newModerationMessage));
    }
}

void Message::setVideoConferenceInfo(const VideoConferenceInfo &info)
{
    if (mBlocks) {
        mBlocks->setVideoConferenceInfo(info);
    }
}

void Message::parseMentions(const QJsonArray &mentions)
{
    QMap<QString, QByteArray> m;

    for (int i = 0; i < mentions.size(); i++) {
        const QJsonObject mention = mentions.at(i).toObject();
        m.insert(mention.value("username"_L1).toString(), mention.value("_id"_L1).toString().toLatin1());
    }
    setMentions(m);
}

void Message::parseMessageUrls(const QJsonArray &urls)
{
    if (!urls.isEmpty()) {
        if (!mUrls) {
            mUrls = new MessageUrls;
        } else {
            mUrls.reset(new MessageUrls);
        }
        mUrls->parseMessageUrls(urls, mMessageId);
    } else {
        mUrls.reset();
    }
}

const Reactions *Message::reactions() const
{
    if (mReactions) {
        return mReactions.data();
    }
    return nullptr;
}

void Message::setReactions(const Reactions &reactions)
{
    if (!mReactions) {
        mReactions = new Reactions(reactions);
    } else {
        mReactions.reset(new Reactions(reactions));
    }
}

bool Message::isPinned() const
{
    if (mMessagePinned) {
        return mMessagePinned->pinned();
    }
    return false;
}

bool Message::isStarred() const
{
    return mMessageStarred.isStarred();
}

void Message::setIsStarred(bool starred)
{
    mMessageStarred.setIsStarred(starred);
}

QMap<QString, QByteArray> Message::mentions() const
{
    return mMentions;
}

void Message::setMentions(const QMap<QString, QByteArray> &mentions)
{
    mMentions = mentions;
}

void Message::parseAttachment(const QJsonArray &attachments)
{
    if (!attachments.isEmpty()) {
        if (!mAttachments) {
            mAttachments = new MessageAttachments;
        } else {
            mAttachments.reset(new MessageAttachments);
        }
        mAttachments->parseMessageAttachments(attachments, messageId());
        if (mAttachments->isEmpty()) {
            mAttachments.reset();
        }
    }
}

bool Message::operator==(const Message &other) const
{
    bool result = (mMessageId == other.messageId()) && (mRoomId == other.roomId()) && (mText == other.text()) && (mTimeStamp == other.timeStamp())
        && (mUsername == other.username()) && (mName == other.name()) && (mUserId == other.userId()) && (mUpdatedAt == other.updatedAt())
        && (mEditedAt == other.editedAt()) && (mEditedByUsername == other.editedByUsername()) && (mAlias == other.alias()) && (mAvatar == other.avatar())
        && (mSystemMessageType == other.systemMessageType()) && (groupable() == other.groupable()) && (parseUrls() == other.parseUrls())
        && (mMentions == other.mentions()) && (mRole == other.role()) && (unread() == other.unread()) && (mMessageStarred == other.messageStarred())
        && (threadCount() == other.threadCount()) && (threadLastMessage() == other.threadLastMessage()) && (discussionCount() == other.discussionCount())
        && (discussionLastMessage() == other.discussionLastMessage()) && (discussionRoomId() == other.discussionRoomId())
        && (threadMessageId() == other.threadMessageId()) && (showTranslatedMessage() == other.showTranslatedMessage()) && (mEmoji == other.emoji())
        && (pendingMessage() == other.pendingMessage()) && (showIgnoredMessage() == other.showIgnoredMessage())
        && (localTranslation() == other.localTranslation()) && (mDisplayTime == other.mDisplayTime) && (privateMessage() == other.privateMessage());
    if (!result) {
        return false;
    }

    // compare urls
    if (replies() && other.replies()) {
        if (*replies() == (*other.replies())) {
            result = true;
        } else {
            return false;
        }
    } else if (!replies() && !other.replies()) {
        result = true;
    } else {
        return false;
    }

    // compare urls
    if (urls() && other.urls()) {
        if (*urls() == (*other.urls())) {
            result = true;
        } else {
            return false;
        }
    } else if (!urls() && !other.urls()) {
        result = true;
    } else {
        return false;
    }

    // compare attachments
    if (attachments() && other.attachments()) {
        if (*attachments() == (*other.attachments())) {
            result = true;
        } else {
            return false;
        }
    } else if (!attachments() && !other.attachments()) {
        result = true;
    } else {
        return false;
    }

    // compare blocks
    if (blocks() && other.blocks()) {
        if (*blocks() == (*other.blocks())) {
            result = true;
        } else {
            return false;
        }
    } else if (!blocks() && !other.blocks()) {
        result = true;
    } else {
        return false;
    }

    // compare channels
    if (channels() && other.channels()) {
        if (*channels() == (*other.channels())) {
            result = true;
        } else {
            return false;
        }
    } else if (!channels() && !other.channels()) {
        result = true;
    } else {
        return false;
    }

    // compare messagePinned
    if (messagePinned() && other.messagePinned()) {
        if (*messagePinned() == (*other.messagePinned())) {
            result = true;
        } else {
            return false;
        }
    } else if (!messagePinned() && !other.messagePinned()) {
        result = true;
    } else {
        return false;
    }

    // compare reactions
    if (reactions() && other.reactions()) {
        if (*reactions() == (*other.reactions())) {
            result = true;
        } else {
            return false;
        }
    } else if (!reactions() && !other.reactions()) {
        result = true;
    } else {
        return false;
    }
    // compare messageTranslation
    if (messageTranslation() && other.messageTranslation()) {
        if (*messageTranslation() == (*other.messageTranslation())) {
            result = true;
        } else {
            return false;
        }
    } else if (!messageTranslation() && !other.messageTranslation()) {
        result = true;
    } else {
        return false;
    }
    // Compare moderationMessage
    if (moderationMessage() && other.moderationMessage()) {
        if (*moderationMessage() == (*other.moderationMessage())) {
            result = true;
        } else {
            return false;
        }
    } else if (!moderationMessage() && !other.moderationMessage()) {
        result = true;
    } else {
        return false;
    }
    return result;
}

bool Message::operator<(const Message &other) const
{
    return mTimeStamp < other.mTimeStamp;
}

SystemMessageTypeUtil::SystemMessageType Message::systemMessageType() const
{
    return mSystemMessageType;
}

void Message::setSystemMessageType(SystemMessageTypeUtil::SystemMessageType systemMessageType)
{
    mSystemMessageType = systemMessageType;
}

Message::MessageType Message::messageType() const
{
    return mMessageType;
}

QString Message::systemMessageText() const
{
    switch (mSystemMessageType) {
    case SystemMessageTypeUtil::SystemMessageType::UserJoined:
        return i18n("%1 has joined the channel", mUsername);
    case SystemMessageTypeUtil::SystemMessageType::UserLeft:
        return i18n("%1 has left the channel", mUsername);
    case SystemMessageTypeUtil::SystemMessageType::UserLeftTeam:
        return i18n("%1 left this Team", mUsername);
    case SystemMessageTypeUtil::SystemMessageType::RoomTopicChanged:
        if (mText.isEmpty()) {
            return i18n("Topic was cleared by: %1", mUsername);
        } else {
            return i18n("%2 changed topic to: <i>%1</i>", mText, mUsername);
        }
    case SystemMessageTypeUtil::SystemMessageType::UserAdded:
        return i18n("%2 added %1 to the conversation", mText, mUsername);
    case SystemMessageTypeUtil::SystemMessageType::RoomNameChanged:
        return i18n("%2 changed room name to <a href=\"ruqola:/room/%1\">#%1</a>", mText, mUsername);
    case SystemMessageTypeUtil::SystemMessageType::UserRemoved:
        return i18n("%2 removed user %1", mText, mUsername);
    case SystemMessageTypeUtil::SystemMessageType::RoomDescriptionChanged:
        if (mText.isEmpty()) {
            return i18n("Description was cleared by %1", mUsername);
        } else {
            return i18n("%2 changed room description to %1", mText, mUsername);
        }
    case SystemMessageTypeUtil::SystemMessageType::RoomAnnoucementChanged:
        if (mText.isEmpty()) {
            return i18n("Announcement was cleared by %1", mUsername);
        } else {
            return i18n("%2 changed room announcement to %1", mText, mUsername);
        }
    case SystemMessageTypeUtil::SystemMessageType::RoomPrivacyChanged:
        return i18n("%2 changed room privacy to %1", mText, mUsername);
    case SystemMessageTypeUtil::SystemMessageType::JitsiCallStarted:
        return u"<a href=\"ruqola:/jitsicall/\">"_s + i18n("Click to join to video") + u"</a>"_s;
    case SystemMessageTypeUtil::SystemMessageType::MessageDeleted:
        // TODO encrypted message
        return i18n("Message Deleted");
    case SystemMessageTypeUtil::SystemMessageType::Pinned:
        return i18n("Message Pinned");
    case SystemMessageTypeUtil::SystemMessageType::EncryptedMessage:
        return i18n("Encrypted Message");
    case SystemMessageTypeUtil::SystemMessageType::UserUnmuted:
        return i18n("%1 was unmuted by %2", mText, mUsername);
    case SystemMessageTypeUtil::SystemMessageType::UserMuted:
        return i18n("%1 was muted by %2", mText, mUsername);
    case SystemMessageTypeUtil::SystemMessageType::SubscriptionRoleAdded:
        return i18n("Role \'%3\' was added to %1 by %2", mText, mUsername, mRole);
    case SystemMessageTypeUtil::SystemMessageType::SubscriptionRoleRemoved:
        return i18n("Role \'%3\' was removed to %1 by %2", mText, mUsername, mRole);
    case SystemMessageTypeUtil::SystemMessageType::MessageE2E:
        // TODO need to unencrypt it
        // return i18n("Encrypted message: %1", mText);
        return i18n("This message is end-to-end encrypted. To view it, you must enter your encryption key in your account settings.");
    case SystemMessageTypeUtil::SystemMessageType::DiscussionCreated:
        return i18n("Discussion created about \"%1\"", mText);
    case SystemMessageTypeUtil::SystemMessageType::UserJoinedConversation:
        return i18n("%1 has joined the conversation", mUsername);
    case SystemMessageTypeUtil::SystemMessageType::RoomArchived:
        return i18n("This room has been archived by %1", mUsername);
    case SystemMessageTypeUtil::SystemMessageType::RoomUnarchived:
        return i18n("This room has been unarchived by %1", mUsername);
    case SystemMessageTypeUtil::SystemMessageType::Rtc:
        qCWarning(RUQOLA_LOG) << "Need to implement : " << mSystemMessageType << " mText " << mText;
        return i18n("Unknown action!");
    case SystemMessageTypeUtil::SystemMessageType::Welcome:
        // TODO verify
        qCWarning(RUQOLA_LOG) << "Need to implement : " << mSystemMessageType << " mText " << mText;
        return i18n("Welcome %1!", mUsername);
    case SystemMessageTypeUtil::SystemMessageType::RoomAvatarChanged:
        return i18n("Room avatar changed by %1", mUsername);
    case SystemMessageTypeUtil::SystemMessageType::RoomE2eEnabled:
        return i18n("This room's encryption has been enabled by %1", mUsername);
    case SystemMessageTypeUtil::SystemMessageType::RoomE2eDisabled:
        return i18n("This room's encryption has been disabled by %1", mUsername);
    case SystemMessageTypeUtil::SystemMessageType::RoomSetReadOnly:
        return i18n("Room set as Read Only by  %1", mUsername);
    case SystemMessageTypeUtil::SystemMessageType::RoomRemoveReadOnly:
        return i18n("Room added writing permission by %1", mUsername);
    case SystemMessageTypeUtil::SystemMessageType::AddedUserToTeam:
        return i18n("%1 added @%2 to this Team", mUsername, mText);
    case SystemMessageTypeUtil::SystemMessageType::RemovedUserFromTeam:
        return i18n("%1 removed @%2 from this Team", mUsername, mText);
    case SystemMessageTypeUtil::SystemMessageType::UserConvertedToTeam:
        return i18n("%1 converted #%2 to a Team", mUsername, mText);
    case SystemMessageTypeUtil::SystemMessageType::UserConvertedToChannel:
        return i18n("%1 converted #%2 to a Channel", mUsername, mText);
    case SystemMessageTypeUtil::SystemMessageType::UserRemovedRoomFromTeam:
        return i18n("%1 removed #%2 from this Team", mUsername, mText);
    case SystemMessageTypeUtil::SystemMessageType::UserDeletedRoomFromTeam:
        return i18n("%1 deleted #%2", mUsername, mText);
    case SystemMessageTypeUtil::SystemMessageType::UserAddedRoomToTeam:
        return i18n("%1 added #%2 to this Team", mUsername, mText);
    case SystemMessageTypeUtil::SystemMessageType::RoomAllowedReacting:
        return i18n("Room allowed reacting by %1", mUsername);
    case SystemMessageTypeUtil::SystemMessageType::RoomDisallowedReacting:
        return i18n("Room disallowed reacting by %1", mUsername);
    case SystemMessageTypeUtil::SystemMessageType::UserJoinedTeam:
        return i18n("%1 joined this Team", mUsername);
    case SystemMessageTypeUtil::SystemMessageType::UserJoinedOtr:
        return i18n("%1 has joined OTR chat.", mUsername);
    case SystemMessageTypeUtil::SystemMessageType::UserKeyRefreshedSuccessfully:
        return i18n("%1 key refreshed successfully", mUsername);
    case SystemMessageTypeUtil::SystemMessageType::UserRequesterOtrKeyRefresh:
        return i18n("%1 has requested key refresh.", mUsername);
    case SystemMessageTypeUtil::SystemMessageType::VideoConf:
        return i18n("Conference Call");
    case SystemMessageTypeUtil::SystemMessageType::Unknown:
        qCWarning(RUQOLA_LOG) << "Unknown type for message: type: " << mSystemMessageType << " mText " << mText;
        break;
    }
    return i18n("Unknown action!");
}

void Message::setMessageType(MessageType messageType)
{
    mMessageType = messageType;
}

const MessageAttachments *Message::attachments() const
{
    if (mAttachments) {
        return mAttachments.data();
    }
    return nullptr;
}

void Message::setAttachments(const MessageAttachments &attachment)
{
    if (!mAttachments) {
        mAttachments = new MessageAttachments(attachment);
    } else {
        mAttachments.reset(new MessageAttachments(attachment));
    }
}

const MessageUrls *Message::urls() const
{
    if (mUrls) {
        return mUrls.data();
    }
    return nullptr;
}

void Message::setUrls(const MessageUrls &urls)
{
    if (urls.isEmpty()) {
        mUrls.reset();
        return;
    }
    if (!mUrls) {
        mUrls = new MessageUrls(urls);
    } else {
        mUrls.reset(new MessageUrls(urls));
    }
}

QString Message::alias() const
{
    return mAlias;
}

void Message::setAlias(const QString &alias)
{
    mAlias = alias;
}

QString Message::editedByUsername() const
{
    return mEditedByUsername;
}

void Message::setEditedByUsername(const QString &editedByUsername)
{
    mEditedByUsername = editedByUsername;
}

bool Message::wasEdited() const
{
    return !mEditedByUsername.isEmpty();
}

qint64 Message::editedAt() const
{
    return mEditedAt;
}

void Message::setEditedAt(qint64 editedAt)
{
    mEditedAt = editedAt;
}

qint64 Message::updatedAt() const
{
    return mUpdatedAt;
}

void Message::setUpdatedAt(qint64 updatedAt)
{
    mUpdatedAt = updatedAt;
}

QByteArray Message::userId() const
{
    return mUserId;
}

void Message::setUserId(const QByteArray &userId)
{
    mUserId = userId;
}

QString Message::username() const
{
    return mUsername;
}

void Message::setUsername(const QString &username)
{
    mUsername = username;
}

qint64 Message::timeStamp() const
{
    return mTimeStamp;
}

void Message::setTimeStamp(qint64 timeStamp)
{
    if (mTimeStamp != timeStamp) {
        mTimeStamp = timeStamp;
        mDisplayTime = QDateTime::fromMSecsSinceEpoch(mTimeStamp).time().toString(u"hh:mm"_s);
    }
}

QString Message::text() const
{
    return mText;
}

void Message::setText(const QString &text)
{
    mText = text;
}

QByteArray Message::messageId() const
{
    return mMessageId;
}

void Message::setMessageId(const QByteArray &messageId)
{
    mMessageId = messageId;
}

QByteArray Message::roomId() const
{
    return mRoomId;
}

void Message::setRoomId(const QByteArray &roomId)
{
    mRoomId = roomId;
}

QString Message::avatar() const
{
    return mAvatar;
}

void Message::setAvatar(const QString &avatar)
{
    mAvatar = avatar;
}

bool Message::parseUrls() const
{
    return messageStateValue(ParsedUrl);
}

void Message::setParseUrls(bool parseUrls)
{
    assignMessageStateValue(ParsedUrl, parseUrls);
}

bool Message::groupable() const
{
    return messageStateValue(Groupable);
}

void Message::setGroupable(bool groupable)
{
    assignMessageStateValue(Groupable, groupable);
}

Utils::AvatarInfo Message::avatarInfo() const
{
    Utils::AvatarInfo info; // Optimize ???
    info.avatarType = Utils::AvatarType::User;
    info.identifier = mUsername;
    return info;
}

Message Message::deserialize(const QJsonObject &o, EmojiManager *emojiManager)
{
    Message message;
    if (o.contains("tcount"_L1)) {
        message.setThreadCount(o["tcount"_L1].toInt());
    }
    if (o.contains("tmid"_L1)) {
        message.setThreadMessageId(o["tmid"_L1].toString().toLatin1());
    }
    if (o.contains("dcount"_L1)) {
        message.setDiscussionCount(o["dcount"_L1].toInt());
    }

    if (o.contains("drid"_L1)) {
        message.setDiscussionRoomId(o.value("drid"_L1).toString().toLatin1());
    }

    message.setPrivateMessage(o["private"_L1].toBool(false));
    if (o.contains("tlm"_L1)) {
        message.setThreadLastMessage(static_cast<qint64>(o["tlm"_L1].toDouble()));
    }
    if (o.contains("dlm"_L1)) {
        message.setDiscussionLastMessage(static_cast<qint64>(o["dlm"_L1].toDouble()));
    }

    message.mMessageId = o["messageID"_L1].toString().toLatin1();
    message.mRoomId = o["roomID"_L1].toString().toLatin1();
    message.mText = o["message"_L1].toString();
    message.setTimeStamp(static_cast<qint64>(o["timestamp"_L1].toDouble()));
    message.mUsername = o["username"_L1].toString();
    message.mName = o["name"_L1].toString();
    message.mUserId = o["userID"_L1].toString().toLatin1();
    message.mUpdatedAt = static_cast<qint64>(o["updatedAt"_L1].toDouble());
    message.setEditedAt(static_cast<qint64>(o["editedAt"_L1].toDouble(-1)));
    message.mEditedByUsername = o["editedByUsername"_L1].toString();
    message.mAlias = o["alias"_L1].toString();
    message.mAvatar = o["avatar"_L1].toString();
    message.setGroupable(o["groupable"_L1].toBool(false));
    message.setParseUrls(o["parseUrls"_L1].toBool(false));
    message.setUnread(o["unread"_L1].toBool());
    message.mMessageStarred.setIsStarred(o["starred"_L1].toBool());

    if (o.contains("pinnedMessage"_L1)) {
        MessagePinned *pinned = MessagePinned::deserialize(o["pinnedMessage"_L1].toObject());
        message.setMessagePinned(*pinned);
        delete pinned;
    }

    message.mRole = o["role"_L1].toString();
    if (o.contains("type"_L1)) {
        message.mSystemMessageType = SystemMessageTypeUtil::systemMessageTypeFromString(o["type"_L1].toString());
    }
    message.mEmoji = o["emoji"_L1].toString();
    message.mMessageType = o["messageType"_L1].toVariant().value<MessageType>();

    if (o.contains("attachments"_L1)) {
        const QJsonArray attachmentsArray = o.value("attachments"_L1).toArray();
        const MessageAttachments *const attachments = MessageAttachments::deserialize(attachmentsArray, message.messageId());
        message.setAttachments(*attachments);
        delete attachments;
    }

    if (o.contains("urls"_L1)) {
        const QJsonArray urlsArray = o.value("urls"_L1).toArray();
        const MessageUrls *const urls = MessageUrls::deserialize(urlsArray, message.messageId());
        message.setUrls(*urls);
        delete urls;
    }

    if (o.contains("reactions"_L1)) {
        const QJsonObject reactionsArray = o.value("reactions"_L1).toObject();
        const Reactions *const reaction = Reactions::deserialize(reactionsArray, emojiManager);
        message.setReactions(*reaction);
        delete reaction;
    }

    if (o.contains("replies"_L1)) {
        const QJsonArray repliesArray = o.value("replies"_L1).toArray();
        const Replies *const replies = Replies::deserialize(repliesArray);
        message.setReplies(*replies);
        delete replies;
    }

    QMap<QString, QByteArray> mentions;
    const QJsonArray mentionsArray = o.value("mentions"_L1).toArray();
    for (int i = 0, total = mentionsArray.count(); i < total; ++i) {
        const QJsonObject mention = mentionsArray.at(i).toObject();
        mentions.insert(mention.value("username"_L1).toString(), mention.value("_id"_L1).toString().toLatin1());
    }
    message.setMentions(mentions);

    if (o.contains("channels"_L1)) {
        const QJsonArray channelsArray = o.value("channels"_L1).toArray();
        const Channels *const channels = Channels::deserialize(channelsArray);
        message.setChannels(*channels);
        delete channels;
    }

    if (o.contains("blocks"_L1)) {
        const QJsonArray blocksArray = o.value("blocks"_L1).toArray();
        const Blocks *const blocks = Blocks::deserialize(blocksArray);
        message.setBlocks(*blocks);
        delete blocks;
    }

    if (o.contains("localTransation"_L1)) {
        message.setLocalTranslation(o["localTransation"_L1].toString());
        message.setShowTranslatedMessage(o["showLocalTranslation"_L1].toBool());
    }

    if (o.contains("messageTranslation"_L1)) {
        const MessageTranslations *const translation = MessageTranslations::deserialize(o["messageTranslation"_L1].toArray());
        message.setMessageTranslation(*translation);
        delete translation;
    }

    return message;
}

QByteArray Message::serialize(const Message &message, bool toBinary)
{
    QJsonDocument d;
    QJsonObject o;

    o["messageID"_L1] = QString::fromLatin1(message.mMessageId);
    o["roomID"_L1] = QString::fromLatin1(message.mRoomId);
    if (!message.mText.isEmpty()) {
        o["message"_L1] = message.mText;
    }
    o["timestamp"_L1] = message.mTimeStamp;
    o["username"_L1] = message.mUsername;
    if (!message.mName.isEmpty()) {
        o["name"_L1] = message.mName;
    }
    o["userID"_L1] = QString::fromLatin1(message.mUserId);
    o["updatedAt"_L1] = message.mUpdatedAt;
    if (message.mEditedAt != -1) {
        o["editedAt"_L1] = message.mEditedAt;
    }
    if (message.threadLastMessage() > -1) {
        o["tlm"_L1] = message.threadLastMessage();
    }

    if (!message.mEditedByUsername.isEmpty()) {
        o["editedByUsername"_L1] = message.mEditedByUsername;
    }
    if (!message.mAlias.isEmpty()) {
        o["alias"_L1] = message.mAlias;
    }
    if (!message.mAvatar.isEmpty()) {
        o["avatar"_L1] = message.mAvatar;
    }
    if (message.groupable()) {
        o["groupable"_L1] = true;
    }
    if (message.parseUrls()) {
        o["parseUrls"_L1] = true;
    }
    if (message.unread()) {
        o["unread"_L1] = true;
    }
    if (message.mMessageStarred.isStarred()) {
        o["starred"_L1] = true;
    }

    if (message.mMessagePinned) {
        o["pinnedMessage"_L1] = MessagePinned::serialize(*message.mMessagePinned);
    }

    if (!message.mRole.isEmpty()) {
        o["role"_L1] = message.mRole;
    }
    if (!message.mEmoji.isEmpty()) {
        o["emoji"_L1] = message.mEmoji;
    }

    if (message.mSystemMessageType != SystemMessageTypeUtil::SystemMessageType::Unknown) {
        if (const QString typeMessage = SystemMessageTypeUtil::systemMessageTypeStringFromEnum(message.mSystemMessageType); !typeMessage.isEmpty()) {
            o["type"_L1] = typeMessage;
        }
    }
    o["messageType"_L1] = QJsonValue::fromVariant(QVariant::fromValue<Message::MessageType>(message.mMessageType));

    // Attachments
    if (message.attachments() && !message.attachments()->isEmpty()) {
        o["attachments"_L1] = MessageAttachments::serialize(*message.attachments());
    }

    // Mentions
    if (!message.mentions().isEmpty()) {
        QJsonArray array;
        for (const auto &[key, value] : message.mentions().asKeyValueRange()) {
            QJsonObject mention;
            mention.insert("_id"_L1, QString::fromLatin1(value));
            mention.insert("username"_L1, key);
            array.append(std::move(mention));
        }
        o["mentions"_L1] = array;
    }

    // Channels
    if (message.channels() && !message.channels()->isEmpty()) {
        o["channels"_L1] = Channels::serialize(*message.channels());
    }

    // Urls
    if (message.urls() && !message.urls()->isEmpty()) {
        o["urls"_L1] = MessageUrls::serialize(*message.urls());
    }

    if (message.reactions() && !message.reactions()->isEmpty()) {
        o["reactions"_L1] = Reactions::serialize(*message.reactions());
    }

    if (message.threadCount() > 0) {
        o["tcount"_L1] = message.threadCount();
        o["tlm"_L1] = message.threadLastMessage();
    }

    if (message.discussionCount() > 0) {
        o["dcount"_L1] = message.discussionCount();
        o["dlm"_L1] = message.discussionLastMessage();
    }
    if (!message.discussionRoomId().isEmpty()) {
        o["drid"_L1] = QString::fromLatin1(message.discussionRoomId());
    }

    if (!message.threadMessageId().isEmpty()) {
        o["tmid"_L1] = QString::fromLatin1(message.threadMessageId());
    }
    if (message.replies() && !message.replies()->isEmpty()) {
        o["replies"_L1] = Replies::serialize(*message.replies());
    }

    // Blocks
    if (message.blocks() && !message.blocks()->isEmpty()) {
        o["blocks"_L1] = Blocks::serialize(*message.blocks());
    }

    if (!message.localTranslation().isEmpty()) {
        o["localTransation"_L1] = message.localTranslation();
        o["showLocalTranslation"_L1] = message.showTranslatedMessage();
    }
    if (message.messageTranslation() && !message.messageTranslation()->isEmpty()) {
        o["messageTranslation"_L1] = MessageTranslations::serialize(*message.messageTranslation());
    }
    if (message.privateMessage()) {
        o["private"_L1] = true;
    }

    if (toBinary) {
        return QCborValue::fromJsonValue(o).toCbor();
    }

    d.setObject(o);
    return d.toJson(QJsonDocument::Indented);
}

QDebug operator<<(QDebug d, const Message &t)
{
    d.space() << "mMessageId:" << t.messageId();
    d.space() << "mText:" << t.text();
    d.space() << "mTimeStamp:" << t.timeStamp();
    d.space() << "mUsername:" << t.username();
    d.space() << "mName:" << t.name();
    d.space() << "mUserId:" << t.userId();
    d.space() << "mUpdatedAt:" << t.updatedAt();
    d.space() << "mEditedAt:" << t.editedAt();
    d.space() << "mEditedByUsername:" << t.editedByUsername();
    d.space() << "mAlias:" << t.alias();
    d.space() << "mSystemMessageType:" << t.systemMessageType();
    d.space() << "mRoomId:" << t.roomId();
    d.space() << "mAvatar:" << t.avatar();
    d.space() << "mGroupable:" << t.groupable();
    d.space() << "mParseUrls:" << t.parseUrls();
    if (t.attachments() && !t.attachments()->isEmpty()) {
        d.space() << "\nAttachment:" << *t.attachments();
    }

    if (t.urls() && !t.urls()->isEmpty()) {
        d.space() << "\nUrls:" << *t.urls();
    }
    d.space() << "Mentions:" << t.mentions();
    d.space() << "mMessageType:" << t.messageType();
    d.space() << "mRole:" << t.role();
    if (t.reactions() && t.reactions()->isEmpty()) {
        d.space() << "mReaction:" << *t.reactions();
    }
    d.space() << "mUnread:" << t.unread();
    d.space() << "starred" << t.messageStarred();
    if (t.messagePinned()) {
        d.space() << "pinned" << *t.messagePinned();
    }
    d.space() << "threadcount" << t.threadCount();
    d.space() << "threadlastmessage" << t.threadLastMessage();
    d.space() << "discussionlastmessage" << t.discussionLastMessage();
    d.space() << "discussionRoomId" << t.discussionRoomId();
    d.space() << "threadMessageId" << t.threadMessageId();
    if (t.messageTranslation()) {
        d.space() << "messagetranslation" << *t.messageTranslation();
    }
    d.space() << "mShowOriginalMessage" << t.showTranslatedMessage();
    if (t.replies()) {
        d.space() << "mReplies" << *t.replies();
    }
    d.space() << "mEmoji" << t.emoji();
    d.space() << "mPendingMessage" << t.pendingMessage();
    d.space() << "mShowIgnoredMessage" << t.showIgnoredMessage();
    if (t.channels()) {
        d.space() << "mChannels" << *t.channels();
    }
    d.space() << "mLocalTranslation" << t.localTranslation();
    d.space() << "mDiscussionRoomId" << t.discussionRoomId();
    d.space() << "mDiscussionCount" << t.discussionCount();
    d.space() << "mDiscussionLastMessage" << t.discussionLastMessage();
    if (t.moderationMessage()) {
        d.space() << "moderationMessage" << *t.moderationMessage();
    }

    if (t.blocks()) {
        d.space() << "block" << *t.blocks();
    }

    d.space() << "mPrivateMessage" << t.privateMessage();
    return d;
}

QString Message::dateTime() const
{
    const QDateTime currentDate = QDateTime::fromMSecsSinceEpoch(timeStamp());
    return QLocale().toString(currentDate, QLocale::ShortFormat);
}

bool Message::messageStateValue(MessageState type) const
{
    return mMessageStates & type;
}

Message::MessageStates Message::messageStates() const
{
    return mMessageStates;
}

void Message::setMessageStates(MessageStates newMessageStates)
{
    mMessageStates = newMessageStates;
}

void Message::assignMessageStateValue(MessageState type, bool status)
{
    if (status) {
        mMessageStates |= type;
    } else {
        mMessageStates &= ~type;
    }
}

#include "moc_message.cpp"
