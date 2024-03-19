/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "block.h"
#include "libruqolacore_export.h"
#include "messageattachment.h"
#include "messageextra.h"
#include "messagepinned.h"
#include "messagestarred.h"
#include "messagetranslation.h"
#include "messageurl.h"
#include "moderation/moderationmessage.h"
#include "reactions.h"
#include "systemmessagetypeutil.h"
#include "utils.h"
#include <QColor>
#include <QList>
#include <QString>

class EmojiManager;
class LIBRUQOLACORE_EXPORT Message
{
    Q_GADGET
public:
    Message();
    Message(const Message &other) = default;
    virtual ~Message();

    enum MessageType {
        System,
        Information,
        NormalText,
        VideoConference,
    };

    Q_ENUM(MessageType)

    enum MessageState {
        None = 0,
        Groupable = 1,
        Unread = 2,
        Pending = 4,
        Private = 8,
        Ignored = 16,
        HoverHighlight = 32,
        Edited = 64,
        Translated = 128,
        ParsedUrl = 256,
    };
    Q_FLAGS(MessageState MessageStates)
    Q_DECLARE_FLAGS(MessageStates, MessageState)

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &roomId);

    [[nodiscard]] bool groupable() const;
    void setGroupable(bool groupable);

    [[nodiscard]] bool parseUrls() const;
    void setParseUrls(bool parseUrls);

    [[nodiscard]] QString avatar() const;
    void setAvatar(const QString &avatar);

    /**
     * @brief Constructs Message object from QJsonObject
     *
     * @param source The Json containing message attributes
     * @return Message object, The message constructed from Json
     */
    [[nodiscard]] static Message deserialize(const QJsonObject &source, EmojiManager *emojiManager = nullptr);

    /**
     * @brief Constructs QBytearray from Message object
     *
     * @param message The Message object
     * @return QByteArray, The Json containing message attributes
     */
    [[nodiscard]] static QByteArray serialize(const Message &message, bool toBinary = true);

    void parseMessage(const QJsonObject &o, bool restApi, EmojiManager *emojiManager);

    [[nodiscard]] bool operator==(const Message &other) const;

    Message &operator=(const Message &other) = default;

    // To be used in sorted insert: timestamp
    bool operator<(const Message &other) const;

    [[nodiscard]] QByteArray messageId() const;
    void setMessageId(const QByteArray &messageId);

    [[nodiscard]] QString text() const;
    void setText(const QString &text);

    [[nodiscard]] qint64 timeStamp() const;
    void setTimeStamp(qint64 timeStamp);

    [[nodiscard]] QString username() const;
    void setUsername(const QString &username);

    [[nodiscard]] QByteArray userId() const;
    void setUserId(const QByteArray &userId);

    [[nodiscard]] qint64 updatedAt() const;
    void setUpdatedAt(qint64 updatedAt);

    [[nodiscard]] qint64 editedAt() const;
    void setEditedAt(qint64 editedAt);

    [[nodiscard]] QString editedByUsername() const;
    void setEditedByUsername(const QString &editedByUsername);

    [[nodiscard]] bool wasEdited() const;

    [[nodiscard]] QString alias() const;
    void setAlias(const QString &alias);

    [[nodiscard]] SystemMessageTypeUtil::SystemMessageType systemMessageType() const;
    void setSystemMessageType(const SystemMessageTypeUtil::SystemMessageType &systemMessageType);

    [[nodiscard]] MessageType messageType() const;
    void setMessageType(Message::MessageType messageType);

    [[nodiscard]] QList<MessageAttachment> attachments() const;
    void setAttachments(const QList<MessageAttachment> &attachments);

    [[nodiscard]] QList<MessageUrl> urls() const;
    void setUrls(const QList<MessageUrl> &urls);

    [[nodiscard]] QMap<QString, QString> mentions() const;
    void setMentions(const QMap<QString, QString> &mentions);

    [[nodiscard]] bool isStarred() const;
    void setIsStarred(bool starred);

    [[nodiscard]] Reactions reactions() const;
    void setReactions(const Reactions &reactions);

    [[nodiscard]] QString systemMessageText() const;

    [[nodiscard]] QString role() const;
    void setRole(const QString &role);

    [[nodiscard]] bool unread() const;
    void setUnread(bool unread);

    [[nodiscard]] MessagePinned messagePinned() const;
    void setMessagePinned(const MessagePinned &messagePinned);

    [[nodiscard]] MessageStarred messageStarred() const;
    void setMessageStarred(MessageStarred messageStarred);

    [[nodiscard]] int threadCount() const;
    void setThreadCount(int threadCount);

    [[nodiscard]] qint64 threadLastMessage() const;
    void setThreadLastMessage(qint64 threadLastMessage);

    [[nodiscard]] qint64 discussionLastMessage() const;
    void setDiscussionLastMessage(qint64 discussionLastMessage);

    [[nodiscard]] int discussionCount() const;
    void setDiscussionCount(int discussionCount);

    [[nodiscard]] QByteArray discussionRoomId() const;
    void setDiscussionRoomId(const QByteArray &discussionRoomId);

    [[nodiscard]] QByteArray threadMessageId() const;
    void setThreadMessageId(const QByteArray &threadMessageId);

    [[nodiscard]] QString displayTime() const;

    [[nodiscard]] MessageTranslation messageTranslation() const;
    void setMessageTranslation(const MessageTranslation &messageTranslation);

    [[nodiscard]] bool showTranslatedMessage() const;
    void setShowTranslatedMessage(bool showTranslatedMessage);

    [[nodiscard]] QString name() const;
    void setName(const QString &name);

    [[nodiscard]] QStringList replies() const;
    void setReplies(const QStringList &replies);

    [[nodiscard]] QString emoji() const;
    void setEmoji(const QString &emoji);

    [[nodiscard]] Utils::AvatarInfo avatarInfo() const;
    [[nodiscard]] bool pendingMessage() const;
    void setPendingMessage(bool pendingMessage);

    [[nodiscard]] bool isPinned() const;
    [[nodiscard]] bool isAutoTranslated() const;
    [[nodiscard]] bool showIgnoredMessage() const;
    void setShowIgnoredMessage(bool showIgnoredMessage);

    [[nodiscard]] bool isEditingMode() const;
    void setIsEditingMode(bool isEditingMode);

    const QMap<QString, QString> &channels() const;
    void setChannels(const QMap<QString, QString> &newChannels);

    [[nodiscard]] bool hoverHighlight() const;
    void setHoverHighlight(bool newShowReactionIcon);

    [[nodiscard]] QString localTranslation() const;
    void setLocalTranslation(const QString &newLocalTranslation);

    [[nodiscard]] QString originalMessageOrAttachmentDescription() const;

    [[nodiscard]] QList<Block> blocks() const;
    void setBlocks(const QList<Block> &newBlocks);

    [[nodiscard]] QString dateTime() const;

    void setVideoConferenceInfo(const VideoConferenceInfo &info);

    [[nodiscard]] const ModerationMessage *moderationMessage() const;
    void setModerationMessage(const ModerationMessage &newModerationMessage);

    [[nodiscard]] bool privateMessage() const;
    void setPrivateMessage(bool newPrivateMessage);

    [[nodiscard]] MessageStates messageStates() const;
    void setMessageStates(const MessageStates &newMessageStates);

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT static QByteArray generateUniqueId(const QByteArray &messageId, int index);
    LIBRUQOLACORE_NO_EXPORT void parseMentions(const QJsonArray &mentions);
    LIBRUQOLACORE_NO_EXPORT void parseAttachment(const QJsonArray &attachments);
    LIBRUQOLACORE_NO_EXPORT void parseUrls(const QJsonArray &urls);
    LIBRUQOLACORE_NO_EXPORT void parseReactions(const QJsonObject &mentions, EmojiManager *emojiManager);
    LIBRUQOLACORE_NO_EXPORT void parseChannels(const QJsonArray &channels);
    LIBRUQOLACORE_NO_EXPORT void parseBlocks(const QJsonArray &blocks);
    LIBRUQOLACORE_NO_EXPORT void assignMessageStateValue(MessageState type, bool status);
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT bool messageStateValue(MessageState type) const;
    [[nodiscard]] MessageExtra *messageExtra();

    // Moderation Message Info
    QSharedDataPointer<ModerationMessage> mModerationMessage;
    QSharedDataPointer<MessageExtra> mMessageExtra;

    // Message Pinned
    MessagePinned mMessagePinned;

    // Message Starred
    MessageStarred mMessageStarred;

    // Message Translation
    MessageTranslation mMessageTranslation;

    // Message Object Fields
    QList<MessageAttachment> mAttachments;

    // Message urls object
    QList<MessageUrl> mUrls;

    // Block
    QList<Block> mBlocks;

    // Reactions
    Reactions mReactions;

    // Mentions username/identifier
    QMap<QString, QString> mMentions;

    // Channels Name/identifier
    QMap<QString, QString> mChannels;

    // Users which replies to thread
    QStringList mReplies;

    // role used when we add/remove role. It will displaying in messagesystem
    QString mRole;

    // _id
    QByteArray mMessageId;

    // msg
    QString mText;

    // u
    QString mUsername;
    QString mName;
    QByteArray mUserId;

    // editedBy
    QString mEditedByUsername;

    // alias
    QString mAlias;

    // rid
    QString mRoomId;

    // avatar
    QString mAvatar;

    // emoji
    QString mEmoji;

    // ts
    QString mDisplayTime;
    qint64 mTimeStamp = -1;
    // _updatedAt
    qint64 mUpdatedAt = -1;

    // editedAt
    qint64 mEditedAt = -1;

    SystemMessageTypeUtil::SystemMessageType mSystemMessageType = SystemMessageTypeUtil::SystemMessageType::Unknown;
    MessageType mMessageType = MessageType::NormalText;
    MessageStates mMessageStates = MessageStates(MessageState::Groupable | MessageState::Translated);
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Message &t);
