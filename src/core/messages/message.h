/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "block.h"
#include "libruqolacore_export.h"
#include "messageattachment.h"
#include "messagepinned.h"
#include "messagestarred.h"
#include "messagetranslation.h"
#include "messageurl.h"
#include "moderation/moderationmessage.h"
#include "reactions.h"
#include "utils.h"
#include <QList>
#include <QString>
class EmojiManager;
class LIBRUQOLACORE_EXPORT Message
{
    Q_GADGET
public:
    explicit Message(EmojiManager *emojiManager = nullptr);
    Message(const Message &other) = default;
    virtual ~Message();

    enum MessageType {
        System,
        Information,
        NormalText,
        VideoConference,
    };

    Q_ENUM(MessageType)

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

    void parseMessage(const QJsonObject &o, bool restApi = false);

    [[nodiscard]] bool operator==(const Message &other) const;

    Message &operator=(const Message &other) = default;

    // To be used in sorted insert: timestamp
    bool operator<(const Message &other) const;

    [[nodiscard]] QString messageId() const;
    void setMessageId(const QString &messageId);

    [[nodiscard]] QString text() const;
    void setText(const QString &text);

    [[nodiscard]] qint64 timeStamp() const;
    void setTimeStamp(qint64 timeStamp);

    [[nodiscard]] QString username() const;
    void setUsername(const QString &username);

    [[nodiscard]] QString userId() const;
    void setUserId(const QString &userId);

    [[nodiscard]] qint64 updatedAt() const;
    void setUpdatedAt(qint64 updatedAt);

    [[nodiscard]] qint64 editedAt() const;
    void setEditedAt(qint64 editedAt);

    [[nodiscard]] QString editedByUsername() const;
    void setEditedByUsername(const QString &editedByUsername);

    [[nodiscard]] bool wasEdited() const;

    [[nodiscard]] QString editedByUserId() const;
    void setEditedByUserId(const QString &editedByUserId);

    [[nodiscard]] QString alias() const;
    void setAlias(const QString &alias);

    [[nodiscard]] QString systemMessageType() const;
    void setSystemMessageType(const QString &systemMessageType);

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

    [[nodiscard]] QString discussionRoomId() const;
    void setDiscussionRoomId(const QString &discussionRoomId);

    [[nodiscard]] QString threadMessageId() const;
    void setThreadMessageId(const QString &threadMessageId);

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

    [[nodiscard]] QString editedDisplayTime() const;

    const QMap<QString, QString> &channels() const;
    void setChannels(const QMap<QString, QString> &newChannels);

    [[nodiscard]] bool hoverHighlight() const;
    void setHoverHighlight(bool newShowReactionIcon);

    [[nodiscard]] const QString &localTranslation() const;
    void setLocalTranslation(const QString &newLocalTranslation);

    [[nodiscard]] QString originalMessageOrAttachmentDescription() const;

    [[nodiscard]] QList<Block> blocks() const;
    void setBlocks(const QList<Block> &newBlocks);

    [[nodiscard]] QString dateTime() const;

    [[nodiscard]] QColor goToMessageBackgroundColor() const;
    void setGoToMessageBackgroundColor(const QColor &newGoToMessageBackgroundColor);

    void setVideoConferenceInfo(const VideoConferenceInfo &info);

    [[nodiscard]] ModerationMessage moderationMessage() const;
    void setModerationMessage(const ModerationMessage &newModerationMessage);

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT static QString generateUniqueId(const QString &messageId, int index);
    LIBRUQOLACORE_NO_EXPORT void parseMentions(const QJsonArray &mentions);
    LIBRUQOLACORE_NO_EXPORT void parseAttachment(const QJsonArray &attachments);
    LIBRUQOLACORE_NO_EXPORT void parseUrls(const QJsonArray &urls);
    LIBRUQOLACORE_NO_EXPORT void parseReactions(const QJsonObject &mentions);
    LIBRUQOLACORE_NO_EXPORT void parseChannels(const QJsonArray &channels);
    LIBRUQOLACORE_NO_EXPORT void parseBlocks(const QJsonArray &blocks);

    // Moderation Message Info
    // Optimization !!!
    ModerationMessage mModerationMessage;

    // BackgroundColor
    QColor mGoToMessageBackgroundColor;

    // Local translation
    QString mLocalTranslation;

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
    QString mMessageId;

    // msg
    QString mText;

    // u
    QString mUsername;
    QString mName;
    QString mUserId;

    // editedBy
    QString mEditedByUsername;
    QString mEditedByUserId;

    // alias
    QString mAlias;

    QString mSystemMessageType;

    // rid
    QString mRoomId;

    // avatar
    QString mAvatar;

    // emoji
    QString mEmoji;

    // drid discussion room id
    QString mDiscussionRoomId;

    // tmid
    QString mThreadMessageId;

    // ts
    QString mDisplayTime;
    qint64 mTimeStamp = -1;
    // _updatedAt
    qint64 mUpdatedAt = -1;

    // editedAt
    QString mEditedDisplayTime;
    qint64 mEditedAt = -1;

    // Thread last message
    qint64 mThreadLastMessage = -1;
    // Thread count
    int mThreadCount = 0;

    // Thread last message
    qint64 mDiscussionLastMessage = -1;
    // Thread count
    int mDiscussionCount = 0;

    MessageType mMessageType = MessageType::NormalText;
    // groupable
    bool mGroupable = true;

    // parseUrls
    bool mParseUrls = false;

    // Unread Message
    bool mUnread = false;

    bool mShowTranslatedMessage = true;

    bool mPendingMessage = false;

    bool mShowIgnoredMessage = false;

    bool mIsEditingMode = false;

    bool mHoverHighlight = false;

    EmojiManager *mEmojiManager = nullptr;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Message &t);
