/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

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
#include "reactions.h"
#include "utils.h"
#include <QJsonObject>
#include <QString>
#include <QVector>
class EmojiManager;
class LIBRUQOLACORE_EXPORT Message
{
    Q_GADGET
public:
    explicit Message(EmojiManager *emojiManager = nullptr);
    Message(const Message &other) = default;

    enum MessageType {
        System,
        Information,
        NormalText,
        VideoConference,
    };

    Q_ENUM(MessageType)

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

    Q_REQUIRED_RESULT bool groupable() const;
    void setGroupable(bool groupable);

    Q_REQUIRED_RESULT bool parseUrls() const;
    void setParseUrls(bool parseUrls);

    Q_REQUIRED_RESULT QString avatar() const;
    void setAvatar(const QString &avatar);

    /**
     * @brief Constructs Message object from QJsonObject
     *
     * @param source The Json containing message attributes
     * @return Message object, The message constructed from Json
     */
    Q_REQUIRED_RESULT static Message fromJSon(const QJsonObject &source, EmojiManager *emojiManager = nullptr);

    /**
     * @brief Constructs QBytearray from Message object
     *
     * @param message The Message object
     * @return QByteArray, The Json containing message attributes
     */
    Q_REQUIRED_RESULT static QByteArray serialize(const Message &message, bool toBinary = true);

    void parseMessage(const QJsonObject &o, bool restApi = false);

    Q_REQUIRED_RESULT bool operator==(const Message &other) const;

    Message &operator=(const Message &other) = default;

    // To be used in sorted insert: timestamp
    bool operator<(const Message &other) const;

    Q_REQUIRED_RESULT QString messageId() const;
    void setMessageId(const QString &messageId);

    Q_REQUIRED_RESULT QString text() const;
    void setText(const QString &text);

    Q_REQUIRED_RESULT qint64 timeStamp() const;
    void setTimeStamp(qint64 timeStamp);

    Q_REQUIRED_RESULT QString username() const;
    void setUsername(const QString &username);

    Q_REQUIRED_RESULT QString userId() const;
    void setUserId(const QString &userId);

    Q_REQUIRED_RESULT qint64 updatedAt() const;
    void setUpdatedAt(qint64 updatedAt);

    Q_REQUIRED_RESULT qint64 editedAt() const;
    void setEditedAt(qint64 editedAt);

    Q_REQUIRED_RESULT QString editedByUsername() const;
    void setEditedByUsername(const QString &editedByUsername);

    Q_REQUIRED_RESULT bool wasEdited() const;

    Q_REQUIRED_RESULT QString editedByUserId() const;
    void setEditedByUserId(const QString &editedByUserId);

    Q_REQUIRED_RESULT QString alias() const;
    void setAlias(const QString &alias);

    Q_REQUIRED_RESULT QString systemMessageType() const;
    void setSystemMessageType(const QString &systemMessageType);

    Q_REQUIRED_RESULT MessageType messageType() const;
    void setMessageType(Message::MessageType messageType);

    Q_REQUIRED_RESULT QVector<MessageAttachment> attachments() const;
    void setAttachments(const QVector<MessageAttachment> &attachments);

    Q_REQUIRED_RESULT QVector<MessageUrl> urls() const;
    void setUrls(const QVector<MessageUrl> &urls);

    Q_REQUIRED_RESULT QMap<QString, QString> mentions() const;
    void setMentions(const QMap<QString, QString> &mentions);

    Q_REQUIRED_RESULT bool isStarred() const;
    void setIsStarred(bool starred);

    Q_REQUIRED_RESULT Reactions reactions() const;
    void setReactions(const Reactions &reactions);

    Q_REQUIRED_RESULT QString systemMessageText() const;

    Q_REQUIRED_RESULT QString role() const;
    void setRole(const QString &role);

    Q_REQUIRED_RESULT bool unread() const;
    void setUnread(bool unread);

    Q_REQUIRED_RESULT MessagePinned messagePinned() const;
    void setMessagePinned(const MessagePinned &messagePinned);

    Q_REQUIRED_RESULT MessageStarred messageStarred() const;
    void setMessageStarred(MessageStarred messageStarred);

    Q_REQUIRED_RESULT int threadCount() const;
    void setThreadCount(int threadCount);

    Q_REQUIRED_RESULT qint64 threadLastMessage() const;
    void setThreadLastMessage(qint64 threadLastMessage);

    Q_REQUIRED_RESULT qint64 discussionLastMessage() const;
    void setDiscussionLastMessage(qint64 discussionLastMessage);

    Q_REQUIRED_RESULT int discussionCount() const;
    void setDiscussionCount(int discussionCount);

    Q_REQUIRED_RESULT QString discussionRoomId() const;
    void setDiscussionRoomId(const QString &discussionRoomId);

    Q_REQUIRED_RESULT QString threadMessageId() const;
    void setThreadMessageId(const QString &threadMessageId);

    Q_REQUIRED_RESULT QString displayTime() const;

    Q_REQUIRED_RESULT MessageTranslation messageTranslation() const;
    void setMessageTranslation(const MessageTranslation &messageTranslation);

    Q_REQUIRED_RESULT bool showTranslatedMessage() const;
    void setShowTranslatedMessage(bool showTranslatedMessage);

    Q_REQUIRED_RESULT bool showAttachment() const;
    void setShowAttachment(bool showAttachment);

    Q_REQUIRED_RESULT QString name() const;
    void setName(const QString &name);

    Q_REQUIRED_RESULT QStringList replies() const;
    void setReplies(const QStringList &replies);

    Q_REQUIRED_RESULT QString emoji() const;
    void setEmoji(const QString &emoji);

    Q_REQUIRED_RESULT Utils::AvatarInfo avatarInfo() const;
    Q_REQUIRED_RESULT bool pendingMessage() const;
    void setPendingMessage(bool pendingMessage);

    Q_REQUIRED_RESULT bool isPinned() const;
    Q_REQUIRED_RESULT bool isAutoTranslated() const;
    Q_REQUIRED_RESULT bool showIgnoredMessage() const;
    void setShowIgnoredMessage(bool showIgnoredMessage);

    Q_REQUIRED_RESULT bool isEditingMode() const;
    void setIsEditingMode(bool isEditingMode);

    Q_REQUIRED_RESULT QString editedDisplayTime() const;

    const QMap<QString, QString> &channels() const;
    void setChannels(const QMap<QString, QString> &newChannels);

    Q_REQUIRED_RESULT bool hoverHighlight() const;
    void setHoverHighlight(bool newShowReactionIcon);

    Q_REQUIRED_RESULT const QString &localTranslation() const;
    void setLocalTranslation(const QString &newLocalTranslation);

    Q_REQUIRED_RESULT QString originalMessageOrAttachmentDescription() const;

    Q_REQUIRED_RESULT QString videoConferenceText() const;

    Q_REQUIRED_RESULT QVector<Block> blocks() const;
    void setBlocks(const QVector<Block> &newBlocks);

    Q_REQUIRED_RESULT QString dateTime() const;

private:
    Q_REQUIRED_RESULT static QString generateAttachmentId(const QString &messageId, int index);
    void parseMentions(const QJsonArray &mentions);
    void parseAttachment(const QJsonArray &attachments);
    void parseUrls(const QJsonArray &urls);
    void parseReactions(const QJsonObject &mentions);
    void parseChannels(const QJsonArray &channels);
    void parseBlocks(const QJsonArray &blocks);

    // Local translation
    QString mLocalTranslation;

    // Message Pinned
    MessagePinned mMessagePinned;

    // Message Starred
    MessageStarred mMessageStarred;

    // Message Translation
    MessageTranslation mMessageTranslation;

    // Message Object Fields
    QVector<MessageAttachment> mAttachments;

    // Message urls object
    QVector<MessageUrl> mUrls;

    // Block
    QVector<Block> mBlocks;

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
Q_DECLARE_METATYPE(Message)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Message &t);
