/*
   SPDX-FileCopyrightText: 2017-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "message.h"
#include "ruqola_debug.h"
#include <KLocalizedString>
#include <QCborValue>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>

Message::Message(EmojiManager *emojiManager)
    : mEmojiManager(emojiManager)
{
}

void Message::parseMessage(const QJsonObject &o, bool restApi)
{
    const QString roomId = o.value(QLatin1String("rid")).toString();

    // t ? I can't find it.
    const QString type = o.value(QLatin1String("t")).toString();

    mMessageId = o.value(QLatin1String("_id")).toString();
    mRoomId = roomId;
    mText = o.value(QLatin1String("msg")).toString();
    if (restApi) {
        mUpdatedAt = Utils::parseIsoDate(QStringLiteral("_updatedAt"), o);
        setEditedAt(Utils::parseIsoDate(QStringLiteral("editedAt"), o));
        setTimeStamp(Utils::parseIsoDate(QStringLiteral("ts"), o));
        mThreadLastMessage = Utils::parseIsoDate(QStringLiteral("tlm"), o);
        mDiscussionLastMessage = Utils::parseIsoDate(QStringLiteral("dlm"), o);
    } else {
        setTimeStamp(Utils::parseDate(QStringLiteral("ts"), o));
        mUpdatedAt = Utils::parseDate(QStringLiteral("_updatedAt"), o);
        setEditedAt(Utils::parseDate(QStringLiteral("editedAt"), o));
        // Verify if a day we will use not restapi for it.
        mThreadLastMessage = Utils::parseDate(QStringLiteral("tlm"), o);
        // Verify if a day we will use not restapi for it.
        mDiscussionLastMessage = Utils::parseDate(QStringLiteral("dlm"), o);
    }

    QStringList lst;
    const QJsonArray replieArray = o.value(QLatin1String("replies")).toArray();
    const auto nbReplieArrayCount{replieArray.count()};
    lst.reserve(nbReplieArrayCount);
    for (auto i = 0; i < nbReplieArrayCount; ++i) {
        lst.append(replieArray.at(i).toVariant().toString());
    }
    mReplies = lst;

    mUsername = o.value(QLatin1String("u")).toObject().value(QLatin1String("username")).toString();
    mName = o.value(QLatin1String("u")).toObject().value(QLatin1String("name")).toString();
    mUserId = o.value(QLatin1String("u")).toObject().value(QLatin1String("_id")).toString();
    mEditedByUsername = o.value(QLatin1String("editedBy")).toObject().value(QLatin1String("username")).toString();
    mEditedByUserId = o.value(QLatin1String("editedBy")).toObject().value(QLatin1String("_id")).toString();
    mAlias = o.value(QLatin1String("alias")).toString();
    mAvatar = o.value(QLatin1String("avatar")).toString();
    mGroupable = o.value(QLatin1String("groupable")).toBool(/*true*/ false); // Laurent, disable for the moment groupable
    mParseUrls = o.value(QLatin1String("parseUrls")).toBool();
    mRole = o.value(QLatin1String("role")).toString();
    mThreadCount = o.value(QLatin1String("tcount")).toInt();
    mDiscussionCount = o.value(QLatin1String("dcount")).toInt();
    mDiscussionRoomId = o.value(QLatin1String("drid")).toString();
    mThreadMessageId = o.value(QLatin1String("tmid")).toString();
    mEmoji = o.value(QLatin1String("emoji")).toString();
    mMessageStarred.parse(o);
    mMessagePinned.parse(o);
    mMessageTranslation.parse(o);

    mMessageType = Message::MessageType::NormalText;
    if (!type.isEmpty()) {
        if (type == QStringLiteral("videoconf")) {
            mMessageType = VideoConference;
        } else {
            mSystemMessageType = type;
            mMessageType = System;
        }
    }
    parseMentions(o.value(QLatin1String("mentions")).toArray());

    parseAttachment(o.value(QLatin1String("attachments")).toArray());
    parseUrls(o.value(QLatin1String("urls")).toArray());
    parseReactions(o.value(QLatin1String("reactions")).toObject());
    parseChannels(o.value(QLatin1String("channels")).toArray());
    // TODO unread element
}

void Message::parseReactions(const QJsonObject &reacts)
{
    if (!reacts.isEmpty()) {
        mReactions.parseReactions(reacts, mEmojiManager);
    }
}

QString Message::editedDisplayTime() const
{
    return mEditedDisplayTime;
}

bool Message::isEditingMode() const
{
    return mIsEditingMode;
}

void Message::setIsEditingMode(bool isEditingMode)
{
    mIsEditingMode = isEditingMode;
}

bool Message::showIgnoredMessage() const
{
    return mShowIgnoredMessage;
}

void Message::setShowIgnoredMessage(bool showIgnoredMessage)
{
    mShowIgnoredMessage = showIgnoredMessage;
}

bool Message::pendingMessage() const
{
    return mPendingMessage;
}

void Message::setPendingMessage(bool pendingMessage)
{
    mPendingMessage = pendingMessage;
}

QString Message::emoji() const
{
    return mEmoji;
}

void Message::setEmoji(const QString &emoji)
{
    mEmoji = emoji;
}

QStringList Message::replies() const
{
    return mReplies;
}

void Message::setReplies(const QStringList &replies)
{
    mReplies = replies;
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
    return !mMessageTranslation.translatedString().isEmpty();
}

bool Message::showTranslatedMessage() const
{
    return mShowTranslatedMessage;
}

void Message::setShowTranslatedMessage(bool showOriginalMessage)
{
    mShowTranslatedMessage = showOriginalMessage;
}

MessageTranslation Message::messageTranslation() const
{
    return mMessageTranslation;
}

void Message::setMessageTranslation(const MessageTranslation &messageTranslation)
{
    mMessageTranslation = messageTranslation;
}

QString Message::displayTime() const
{
    return mDisplayTime;
}

QString Message::threadMessageId() const
{
    return mThreadMessageId;
}

void Message::setThreadMessageId(const QString &threadMessageId)
{
    mThreadMessageId = threadMessageId;
}

QString Message::discussionRoomId() const
{
    return mDiscussionRoomId;
}

void Message::setDiscussionRoomId(const QString &discussionRoomId)
{
    mDiscussionRoomId = discussionRoomId;
}

int Message::discussionCount() const
{
    return mDiscussionCount;
}

void Message::setDiscussionCount(int discussionCount)
{
    mDiscussionCount = discussionCount;
}

qint64 Message::discussionLastMessage() const
{
    return mDiscussionLastMessage;
}

void Message::setDiscussionLastMessage(qint64 discussionLastMessage)
{
    mDiscussionLastMessage = discussionLastMessage;
}

qint64 Message::threadLastMessage() const
{
    return mThreadLastMessage;
}

void Message::setThreadLastMessage(qint64 threadLastMessage)
{
    mThreadLastMessage = threadLastMessage;
}

int Message::threadCount() const
{
    return mThreadCount;
}

void Message::setThreadCount(int threadCount)
{
    mThreadCount = threadCount;
}

MessageStarred Message::messageStarred() const
{
    return mMessageStarred;
}

void Message::setMessageStarred(MessageStarred messageStarred)
{
    mMessageStarred = messageStarred;
}

MessagePinned Message::messagePinned() const
{
    return mMessagePinned;
}

void Message::setMessagePinned(const MessagePinned &messagePinned)
{
    mMessagePinned = messagePinned;
}

bool Message::unread() const
{
    return mUnread;
}

void Message::setUnread(bool unread)
{
    mUnread = unread;
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
    mChannels.clear();
    for (int i = 0, total = channels.size(); i < total; ++i) {
        const QJsonObject mention = channels.at(i).toObject();
        mChannels.insert(mention.value(QLatin1String("name")).toString(), mention.value(QLatin1String("_id")).toString());
    }
}

QVector<Block> Message::blocks() const
{
    return mBlocks;
}

void Message::setBlocks(const QVector<Block> &newBlocks)
{
    mBlocks = newBlocks;
}

QString Message::originalMessageOrAttachmentDescription() const
{
    if (attachments().empty()) {
        return text();
    }
    return attachments().constFirst().description();
}

const QString &Message::localTranslation() const
{
    return mLocalTranslation;
}

void Message::setLocalTranslation(const QString &newLocalTranslation)
{
    mLocalTranslation = newLocalTranslation;
}

bool Message::showReactionIcon() const
{
    return mShowReactionIcon;
}

void Message::setShowReactionIcon(bool newShowReactionIcon)
{
    mShowReactionIcon = newShowReactionIcon;
}

const QMap<QString, QString> &Message::channels() const
{
    return mChannels;
}

void Message::setChannels(const QMap<QString, QString> &newChannels)
{
    mChannels = newChannels;
}

void Message::parseMentions(const QJsonArray &mentions)
{
    mMentions.clear();
    for (int i = 0; i < mentions.size(); i++) {
        const QJsonObject mention = mentions.at(i).toObject();
        mMentions.insert(mention.value(QLatin1String("username")).toString(), mention.value(QLatin1String("_id")).toString());
    }
}

void Message::parseUrls(const QJsonArray &urls)
{
    mUrls.clear();
    if (!urls.isEmpty()) {
        qCDebug(RUQOLA_LOG) << " void Message::urls(const QJsonObject &attachments)" << urls;
        for (int i = 0; i < urls.size(); i++) {
            const QJsonObject url = urls.at(i).toObject();
            MessageUrl messageUrl;
            const QJsonValue urlStr = url.value(QLatin1String("url"));
            if (!urlStr.isUndefined()) {
                messageUrl.setUrl(urlStr.toString());
            }
            const QJsonObject meta = url.value(QLatin1String("meta")).toObject();
            if (!meta.isEmpty()) {
                const QJsonValue pageTitleStr = meta.value(QLatin1String("pageTitle"));
                if (!pageTitleStr.isUndefined()) {
                    messageUrl.setPageTitle(pageTitleStr.toString());
                }
                const QJsonValue descriptionStr = meta.value(QLatin1String("description"));
                if (!descriptionStr.isUndefined()) {
                    messageUrl.setDescription(descriptionStr.toString());
                }
            }
            // TODO add more

            if (!messageUrl.isEmpty()) {
                mUrls.append(messageUrl);
            }
        }
    }
}

Reactions Message::reactions() const
{
    return mReactions;
}

void Message::setReactions(const Reactions &reactions)
{
    mReactions = reactions;
}

bool Message::isPinned() const
{
    return mMessagePinned.pinned();
}

bool Message::isStarred() const
{
    return mMessageStarred.isStarred();
}

void Message::setIsStarred(bool starred)
{
    mMessageStarred.setIsStarred(starred);
}

QMap<QString, QString> Message::mentions() const
{
    return mMentions;
}

void Message::setMentions(const QMap<QString, QString> &mentions)
{
    mMentions = mentions;
}

void Message::parseAttachment(const QJsonArray &attachments)
{
    mAttachments.clear();
    if (!attachments.isEmpty()) {
        // qDebug() << " void Message::parseAttachment(const QJsonObject &attachments)"<<attachments;
        for (int i = 0; i < attachments.size(); i++) {
            const QJsonObject attachment = attachments.at(i).toObject();
            MessageAttachment messageAttachement;
            messageAttachement.parseAttachment(attachment);
            messageAttachement.setAttachmentId(Message::generateAttachmentId(messageId(), i));
            if (messageAttachement.isValid()) {
                mAttachments.append(messageAttachement);
            }
        }
    }
}

bool Message::operator==(const Message &other) const
{
    return (mMessageId == other.messageId()) && (mRoomId == other.roomId()) && (mText == other.text()) && (mTimeStamp == other.timeStamp())
        && (mUsername == other.username()) && (mName == other.name()) && (mUserId == other.userId()) && (mUpdatedAt == other.updatedAt())
        && (mEditedAt == other.editedAt()) && (mEditedByUsername == other.editedByUsername()) && (mEditedByUserId == other.editedByUserId())
        && (mAlias == other.alias()) && (mAvatar == other.avatar()) && (mSystemMessageType == other.systemMessageType()) && (mGroupable == other.groupable())
        && (mParseUrls == other.parseUrls()) && (mUrls == other.urls()) && (mAttachments == other.attachments()) && (mMentions == other.mentions())
        && (mRole == other.role()) && (mReactions == other.reactions()) && (mUnread == other.unread()) && (mMessagePinned == other.messagePinned())
        && (mMessageStarred == other.messageStarred()) && (mThreadCount == other.threadCount()) && (mThreadLastMessage == other.threadLastMessage())
        && (mDiscussionCount == other.discussionCount()) && (mDiscussionLastMessage == other.discussionLastMessage())
        && (mDiscussionRoomId == other.discussionRoomId()) && (mThreadMessageId == other.threadMessageId())
        && (mMessageTranslation == other.messageTranslation()) && (mShowTranslatedMessage == other.showTranslatedMessage()) && (mReplies == other.replies())
        && (mEmoji == other.emoji()) && (mPendingMessage == other.pendingMessage()) && (mShowIgnoredMessage == other.showIgnoredMessage())
        && (mChannels == other.channels()) && (mLocalTranslation == other.localTranslation());
}

bool Message::operator<(const Message &other) const
{
    return mTimeStamp < other.mTimeStamp;
}

QString Message::systemMessageType() const
{
    return mSystemMessageType;
}

void Message::setSystemMessageType(const QString &systemMessageType)
{
    mSystemMessageType = systemMessageType;
}

Message::MessageType Message::messageType() const
{
    return mMessageType;
}

QString Message::videoConferenceText() const
{
    return i18n("Conference Call");
}

QString Message::systemMessageText() const
{
    if (mSystemMessageType == QLatin1String("uj")) {
        return i18n("%1 has joined the channel", mUsername);
    } else if (mSystemMessageType == QLatin1String("ul")) {
        return i18n("%1 has left the channel", mUsername);
    } else if (mSystemMessageType == QLatin1String("ult")) {
        return i18n("%1 left this Team", mUsername);
    } else if (mSystemMessageType == QLatin1String("room_changed_topic")) {
        if (mText.isEmpty()) {
            return i18n("Topic was cleared by: %1", mUsername);
        } else {
            return i18n("%2 changed topic to: <i>%1</i>", mText, mUsername);
        }
    } else if (mSystemMessageType == QLatin1String("au")) {
        return i18n("%2 added %1 to the conversation", mText, mUsername);
    } else if (mSystemMessageType == QLatin1Char('r')) {
        return i18n("%2 changed room name to <a href=\"ruqola:/room/%1\">#%1</a>", mText, mUsername);
    } else if (mSystemMessageType == QLatin1String("ru")) {
        return i18n("%2 removed user %1", mText, mUsername);
    } else if (mSystemMessageType == QLatin1String("room_changed_description")) {
        if (mText.isEmpty()) {
            return i18n("Description was cleared by %1", mUsername);
        } else {
            return i18n("%2 changed room description to %1", mText, mUsername);
        }
    } else if (mSystemMessageType == QLatin1String("room_changed_announcement")) {
        if (mText.isEmpty()) {
            return i18n("Announcement was cleared by %1", mUsername);
        } else {
            return i18n("%2 changed room announcement to %1", mText, mUsername);
        }
    } else if (mSystemMessageType == QLatin1String("room_changed_privacy")) {
        return i18n("%2 changed room privacy to %1", mText, mUsername);
    } else if (mSystemMessageType == QLatin1String("jitsi_call_started")) {
        return QStringLiteral("<a href=\"ruqola:/jitsicall/\">") + i18n("Click to join to video") + QStringLiteral("</a>");
    } else if (mSystemMessageType == QLatin1String("rm")) {
        // TODO encrypted message
        return i18n("Message Deleted");
    } else if (mSystemMessageType == QLatin1String("message_pinned")) {
        return i18n("Message Pinned");
    } else if (mSystemMessageType == QLatin1String("otr")) {
        return i18n("Encrypted Message");
    } else if (mSystemMessageType == QLatin1String("user-unmuted")) {
        return i18n("%1 was unmuted by %2", mText, mUsername);
    } else if (mSystemMessageType == QLatin1String("user-muted")) {
        return i18n("%1 was muted by %2", mText, mUsername);
    } else if (mSystemMessageType == QLatin1String("subscription-role-added")) {
        return i18n("Role \'%3\' was added to %1 by %2", mText, mUsername, mRole);
    } else if (mSystemMessageType == QLatin1String("subscription-role-removed")) {
        return i18n("Role \'%3\' was removed to %1 by %2", mText, mUsername, mRole);
    } else if (mSystemMessageType == QLatin1String("e2e")) {
        // TODO need to unencrypt it
        // return i18n("Encrypted message: %1", mText);
        return i18n("This message is end-to-end encrypted. To view it, you must enter your encryption key in your account settings.");
    } else if (mSystemMessageType == QLatin1String("discussion-created")) {
        return i18n("Discussion created about \"%1\"", mText);
    } else if (mSystemMessageType == QLatin1String("ut")) {
        return i18n("%1 has joined the conversation", mUsername);
    } else if (mSystemMessageType == QLatin1String("room-archived")) {
        return i18n("This room has been archived by %1", mUsername);
    } else if (mSystemMessageType == QLatin1String("room-unarchived")) {
        return i18n("This room has been unarchived by %1", mUsername);
    } else if (mSystemMessageType == QLatin1String("rtc")) {
        qCWarning(RUQOLA_LOG) << "Need to implement : " << mSystemMessageType << " mText " << mText;
        return i18n("Unknown action!");
    } else if (mSystemMessageType == QLatin1String("wm")) {
        // TODO verify
        qCWarning(RUQOLA_LOG) << "Need to implement : " << mSystemMessageType << " mText " << mText;
        return i18n("Welcome %1!", mUsername);
    } else if (mSystemMessageType == QLatin1String("room_changed_avatar")) {
        return i18n("Room avatar changed by %1", mUsername);
    } else if (mSystemMessageType == QLatin1String("room_e2e_enabled")) {
        return i18n("This room's encryption has been enabled by %1", mUsername);
    } else if (mSystemMessageType == QLatin1String("room_e2e_disabled")) {
        return i18n("This room's encryption has been disabled by %1", mUsername);
    } else if (mSystemMessageType == QLatin1String("room-set-read-only")) {
        return i18n("Room set as Read Only by  %1", mUsername);
    } else if (mSystemMessageType == QLatin1String("room-removed-read-only")) {
        return i18n("Room added writing permission by %1", mUsername);
    } else if (mSystemMessageType == QLatin1String("added-user-to-team")) {
        return i18n("%1 added @%2 to this Team", mUsername, mText);
    } else if (mSystemMessageType == QLatin1String("removed-user-from-team")) {
        return i18n("%1 removed @%2 from this Team", mUsername, mText);
    } else if (mSystemMessageType == QLatin1String("user-converted-to-team")) {
        return i18n("%1 converted #%2 to a Team", mUsername, mText);
    } else if (mSystemMessageType == QLatin1String("user-converted-to-channel")) {
        return i18n("%1 converted #%2 to a Channel", mUsername, mText);
    } else if (mSystemMessageType == QLatin1String("user-removed-room-from-team")) {
        return i18n("%1 removed #%2 from this Team", mUsername, mText);
    } else if (mSystemMessageType == QLatin1String("user-deleted-room-from-team")) {
        return i18n("%1 deleted #%2", mUsername, mText);
    } else if (mSystemMessageType == QLatin1String("user-added-room-to-team")) {
        return i18n("%1 added #%2 to this Team", mUsername, mText);
    } else if (mSystemMessageType == QLatin1String("room-allowed-reacting")) {
        return i18n("Room allowed reacting by %1", mUsername);
    } else if (mSystemMessageType == QLatin1String("room-disallowed-reacting")) {
        return i18n("Room disallowed reacting by %1", mUsername);
    } else if (mSystemMessageType == QLatin1String("ujt")) {
        return i18n("%1 joined this Team", mUsername);
    } else if (mSystemMessageType == QLatin1String("user_joined_otr")) {
        return i18n("Has joined OTR chat."); // Verify
    } else if (mSystemMessageType == QLatin1String("user_key_refreshed_successfully")) {
        return i18n("key refreshed successfully"); // Verify
    } else if (mSystemMessageType == QLatin1String("user_requested_otr_key_refresh")) {
        return i18n("Has requested key refresh."); // Verify
    } else if (mSystemMessageType == QLatin1String("room_changed_topic")) {
        return i18n("Room topic changed to: %1 by %2", mUsername, mText); // TODO verify
    } else if (mSystemMessageType == QLatin1String("videoconf")) {
        return i18n("Conference Call");
    } else {
        qCWarning(RUQOLA_LOG) << "Unknown type for message: type: " << mSystemMessageType << " mText " << mText;
        return i18n("Unknown action!");
    }
}

void Message::setMessageType(MessageType messageType)
{
    mMessageType = messageType;
}

QVector<MessageAttachment> Message::attachments() const
{
    return mAttachments;
}

void Message::setAttachments(const QVector<MessageAttachment> &attachments)
{
    mAttachments = attachments;
}

QVector<MessageUrl> Message::urls() const
{
    return mUrls;
}

void Message::setUrls(const QVector<MessageUrl> &urls)
{
    mUrls = urls;
}

QString Message::alias() const
{
    return mAlias;
}

void Message::setAlias(const QString &alias)
{
    mAlias = alias;
}

QString Message::editedByUserId() const
{
    return mEditedByUserId;
}

void Message::setEditedByUserId(const QString &editedByUserId)
{
    mEditedByUserId = editedByUserId;
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
    if (mEditedAt != editedAt) {
        mEditedAt = editedAt;
        QLocale l;
        mEditedDisplayTime = l.toString(QDateTime::fromMSecsSinceEpoch(mEditedAt), QLocale::LongFormat);
    }
}

qint64 Message::updatedAt() const
{
    return mUpdatedAt;
}

void Message::setUpdatedAt(qint64 updatedAt)
{
    mUpdatedAt = updatedAt;
}

QString Message::userId() const
{
    return mUserId;
}

void Message::setUserId(const QString &userId)
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
        mDisplayTime = QDateTime::fromMSecsSinceEpoch(mTimeStamp).time().toString(QStringLiteral("hh:mm"));
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

QString Message::messageId() const
{
    return mMessageId;
}

void Message::setMessageId(const QString &messageId)
{
    mMessageId = messageId;
}

QString Message::roomId() const
{
    return mRoomId;
}

void Message::setRoomId(const QString &roomId)
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
    return mParseUrls;
}

void Message::setParseUrls(bool parseUrls)
{
    mParseUrls = parseUrls;
}

bool Message::groupable() const
{
    return mGroupable;
}

void Message::setGroupable(bool groupable)
{
    mGroupable = groupable;
}

QString Message::generateAttachmentId(const QString &messageId, int index)
{
    return QStringLiteral("%1_%2").arg(messageId, QString::number(index));
}

Utils::AvatarInfo Message::avatarInfo() const
{
    Utils::AvatarInfo info; // Optimize ???
    info.avatarType = Utils::AvatarType::User;
    info.identifier = mUsername;
    return info;
}

Message Message::fromJSon(const QJsonObject &o, EmojiManager *emojiManager)
{
    Message message(emojiManager);
    message.mThreadCount = o[QStringLiteral("tcount")].toString().toInt();
    message.mDiscussionCount = o[QStringLiteral("dcount")].toString().toInt();
    message.mDiscussionRoomId = o[QStringLiteral("drid")].toString();
    message.mThreadMessageId = o[QStringLiteral("tmid")].toString();

    if (o.contains(QLatin1String("tlm"))) {
        message.mThreadLastMessage = static_cast<qint64>(o[QStringLiteral("tlm")].toDouble());
    }
    if (o.contains(QLatin1String("dlm"))) {
        message.mDiscussionLastMessage = static_cast<qint64>(o[QStringLiteral("dlm")].toDouble());
    }

    message.mMessageId = o[QStringLiteral("messageID")].toString();
    message.mRoomId = o[QStringLiteral("roomID")].toString();
    message.mText = o[QStringLiteral("message")].toString();
    message.mTimeStamp = static_cast<qint64>(o[QStringLiteral("timestamp")].toDouble());
    message.mUsername = o[QStringLiteral("username")].toString();
    message.mName = o[QStringLiteral("name")].toString();
    message.mUserId = o[QStringLiteral("userID")].toString();
    message.mUpdatedAt = static_cast<qint64>(o[QStringLiteral("updatedAt")].toDouble());
    message.setEditedAt(static_cast<qint64>(o[QStringLiteral("editedAt")].toDouble()));
    message.mEditedByUsername = o[QStringLiteral("editedByUsername")].toString();
    message.mEditedByUserId = o[QStringLiteral("editedByUserID")].toString();
    message.mAlias = o[QStringLiteral("alias")].toString();
    message.mAvatar = o[QStringLiteral("avatar")].toString();
    message.mGroupable = o[QStringLiteral("groupable")].toBool();
    message.mParseUrls = o[QStringLiteral("parseUrls")].toBool();
    message.mMessageStarred.setIsStarred(o[QStringLiteral("starred")].toBool());
    message.mMessagePinned.setPinned(o[QStringLiteral("pinned")].toBool());
    message.mMessagePinned.setPinnedBy(o[QStringLiteral("pinnedBy")].toString());
    message.mRole = o[QStringLiteral("role")].toString();
    message.mSystemMessageType = o[QStringLiteral("type")].toString();
    message.mEmoji = o[QStringLiteral("emoji")].toString();
    message.mMessageType = o[QStringLiteral("messageType")].toVariant().value<MessageType>();
    const QJsonArray attachmentsArray = o.value(QLatin1String("attachments")).toArray();
    for (int i = 0; i < attachmentsArray.count(); ++i) {
        const QJsonObject attachment = attachmentsArray.at(i).toObject();
        MessageAttachment att = MessageAttachment::fromJson(attachment);
        att.setAttachmentId(Message::generateAttachmentId(message.messageId(), i));
        if (att.isValid()) {
            message.mAttachments.append(std::move(att));
        }
    }
    const QJsonArray urlsArray = o.value(QLatin1String("urls")).toArray();
    for (int i = 0; i < urlsArray.count(); ++i) {
        const QJsonObject urlObj = urlsArray.at(i).toObject();
        const MessageUrl url = MessageUrl::fromJSon(urlObj);
        if (!url.isEmpty()) {
            message.mUrls.append(std::move(url));
        }
    }
    const QJsonObject reactionsArray = o.value(QLatin1String("reactions")).toObject();
    message.setReactions(Reactions::fromJSon(reactionsArray, emojiManager));

    const QJsonArray repliesArray = o.value(QLatin1String("replies")).toArray();
    QStringList replies;
    replies.reserve(repliesArray.count());
    for (int i = 0; i < repliesArray.count(); ++i) {
        replies.append(urlsArray.at(i).toVariant().toString());
    }
    message.setReplies(replies);

    if (!message.mReplies.isEmpty()) {
        o[QStringLiteral("replies")] = QJsonArray::fromStringList(message.mReplies);
    }

    QMap<QString, QString> mentions;
    const QJsonArray mentionsArray = o.value(QLatin1String("mentions")).toArray();
    for (int i = 0; i < mentionsArray.count(); ++i) {
        const QJsonObject mention = mentionsArray.at(i).toObject();
        mentions.insert(mention.value(QLatin1String("username")).toString(), mention.value(QLatin1String("_id")).toString());
    }
    message.setMentions(mentions);

    QMap<QString, QString> channels;
    const QJsonArray channelsArray = o.value(QLatin1String("channels")).toArray();
    for (int i = 0; i < channelsArray.count(); ++i) {
        const QJsonObject channel = channelsArray.at(i).toObject();
        channels.insert(channel.value(QLatin1String("channel")).toString(), channel.value(QLatin1String("_id")).toString());
    }
    message.setChannels(channels);

    // TODO add message translation !

    return message;
}

QByteArray Message::serialize(const Message &message, bool toBinary)
{
    QJsonDocument d;
    QJsonObject o;

    o[QStringLiteral("messageID")] = message.mMessageId;
    o[QStringLiteral("roomID")] = message.mRoomId;
    o[QStringLiteral("message")] = message.mText;
    o[QStringLiteral("timestamp")] = message.mTimeStamp;
    o[QStringLiteral("username")] = message.mUsername;
    if (!message.mName.isEmpty()) {
        o[QStringLiteral("name")] = message.mName;
    }
    o[QStringLiteral("userID")] = message.mUserId;
    o[QStringLiteral("updatedAt")] = message.mUpdatedAt;
    o[QStringLiteral("editedAt")] = message.mEditedAt;
    if (message.mThreadLastMessage > -1) {
        o[QStringLiteral("tlm")] = message.mThreadLastMessage;
    }
    if (message.mDiscussionLastMessage > -1) {
        o[QStringLiteral("dlm")] = message.mDiscussionLastMessage;
    }

    o[QStringLiteral("editedByUsername")] = message.mEditedByUsername;
    o[QStringLiteral("editedByUserID")] = message.mEditedByUserId;
    o[QStringLiteral("alias")] = message.mAlias;
    o[QStringLiteral("avatar")] = message.mAvatar;
    o[QStringLiteral("groupable")] = message.mGroupable;
    o[QStringLiteral("parseUrls")] = message.mParseUrls;
    o[QStringLiteral("starred")] = message.mMessageStarred.isStarred();
    o[QStringLiteral("pinned")] = message.mMessagePinned.pinned();
    if (!message.mMessagePinned.pinnedBy().isEmpty()) {
        o[QStringLiteral("pinnedBy")] = message.mMessagePinned.pinnedBy();
    }
    if (!message.mRole.isEmpty()) {
        o[QStringLiteral("role")] = message.mRole;
    }
    if (!message.mEmoji.isEmpty()) {
        o[QStringLiteral("emoji")] = message.mEmoji;
    }

    o[QStringLiteral("type")] = message.mSystemMessageType;
    o[QStringLiteral("messageType")] = QJsonValue::fromVariant(QVariant::fromValue<Message::MessageType>(message.mMessageType));

    // Attachments
    if (!message.mAttachments.isEmpty()) {
        QJsonArray array;
        const int nbAttachment = message.mAttachments.count();
        for (int i = 0; i < nbAttachment; ++i) {
            array.append(MessageAttachment::serialize(message.mAttachments.at(i)));
        }
        o[QStringLiteral("attachments")] = array;
    }

    // Mentions
    if (!message.mentions().isEmpty()) {
        QMapIterator<QString, QString> i(message.mentions());
        QJsonArray array;
        while (i.hasNext()) {
            i.next();
            QJsonObject mention;
            mention.insert(QLatin1String("_id"), i.value());
            mention.insert(QLatin1String("username"), i.key());
            array.append(mention);
        }
        o[QStringLiteral("mentions")] = array;
    }

    // Channels
    if (!message.channels().isEmpty()) {
        QMapIterator<QString, QString> j(message.channels());
        QJsonArray array;
        while (j.hasNext()) {
            j.next();
            QJsonObject channel;
            channel.insert(QLatin1String("_id"), j.value());
            channel.insert(QLatin1String("channel"), j.key());
            array.append(channel);
        }
        o[QStringLiteral("channels")] = array;
    }
    // Urls
    if (!message.mUrls.isEmpty()) {
        QJsonArray array;
        const int nbUrls = message.mUrls.count();
        for (int i = 0; i < nbUrls; ++i) {
            array.append(MessageUrl::serialize(message.mUrls.at(i)));
        }
        o[QStringLiteral("urls")] = array;
    }

    if (!message.reactions().isEmpty()) {
        o[QStringLiteral("reactions")] = Reactions::serialize(message.reactions());
    }

    if (message.mThreadCount > 0) {
        o[QStringLiteral("tcount")] = message.mThreadCount;
        o[QStringLiteral("tlm")] = message.mThreadLastMessage;
    }

    if (message.mDiscussionCount > 0) {
        o[QStringLiteral("dcount")] = message.mDiscussionCount;
        o[QStringLiteral("dlm")] = message.mDiscussionLastMessage;
    }
    if (!message.mDiscussionRoomId.isEmpty()) {
        o[QStringLiteral("drid")] = message.mDiscussionRoomId;
    }

    if (!message.mThreadMessageId.isEmpty()) {
        o[QStringLiteral("tmid")] = message.mThreadMessageId;
    }
    if (!message.mReplies.isEmpty()) {
        o[QStringLiteral("replies")] = QJsonArray::fromStringList(message.mReplies);
    }
    if (toBinary) {
        return QCborValue::fromJsonValue(o).toCbor();
    }
    // TODO add message translation
    d.setObject(o);
    return d.toJson(QJsonDocument::Indented);
}

QDebug operator<<(QDebug d, const Message &t)
{
    d << "mMessageId : " << t.messageId();
    d << "mText: " << t.text();
    d << "mTimeStamp: " << t.timeStamp();
    d << "mUsername: " << t.username();
    d << "mName: " << t.name();
    d << "mUserId: " << t.userId();
    d << "mUpdatedAt: " << t.updatedAt();
    d << "mEditedAt: " << t.editedAt();
    d << "mEditedByUsername: " << t.editedByUsername();
    d << "mEditedByUserId: " << t.editedByUserId();
    d << "mAlias: " << t.alias();
    d << "mSystemMessageType: " << t.systemMessageType();
    d << "mRoomId: " << t.roomId();
    d << "mAvatar: " << t.avatar();
    d << "mGroupable: " << t.groupable();
    d << "mParseUrls: " << t.parseUrls();
    for (int i = 0, total = t.attachments().count(); i < total; ++i) {
        d << "Attachment :" << t.attachments().at(i);
    }
    for (int i = 0, total = t.urls().count(); i < total; ++i) {
        d << "Urls :" << t.urls().at(i);
    }
    d << "Mentions :" << t.mentions();
    d << "mMessageType: " << t.messageType();
    d << "mRole: " << t.role();
    d << "mReaction: " << t.reactions();
    d << "mUnread: " << t.unread();
    d << "starred " << t.messageStarred();
    d << "pinned " << t.messagePinned();
    d << "threadcount " << t.threadCount();
    d << "threadlastmessage " << t.threadLastMessage();
    d << "discussioncount " << t.discussionCount();
    d << "discussionlastmessage " << t.discussionLastMessage();
    d << "discussionRoomId " << t.discussionRoomId();
    d << "threadMessageId " << t.threadMessageId();
    d << "messagetranslation" << t.messageTranslation();
    d << "mShowOriginalMessage " << t.showTranslatedMessage();
    d << "mReplies " << t.replies();
    d << "mEmoji " << t.emoji();
    d << "mPendingMessage " << t.pendingMessage();
    d << "mShowIgnoredMessage " << t.showIgnoredMessage();
    d << "mChannels " << t.channels();
    d << "mLocalTranslation " << t.localTranslation();
    return d;
}
