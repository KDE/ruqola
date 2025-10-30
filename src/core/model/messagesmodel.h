/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#pragma once

#include "libruqolacore_export.h"
#include "messages/message.h"
#include <QAbstractListModel>
#include <QPointer>

class RocketChatAccount;
class LoadRecentHistoryManager;
class Room;
class RuqolaQuickSearchMessageSettings;
class LIBRUQOLACORE_EXPORT MessagesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    struct LIBRUQOLACORE_EXPORT AttachmentAndUrlPreviewVisibility {
        bool show = false;
        QByteArray elementId;
    };

    struct LIBRUQOLACORE_EXPORT HighlightSearchStringIndexInMessage {
        int index = -1;
        QByteArray messageId;
        void clear();
        [[nodiscard]] bool operator==(const HighlightSearchStringIndexInMessage &other) const;
    };

    enum MessageRoles {
        Username = Qt::UserRole + 1,
        MessagePointer,
        OriginalMessage,
        MessageConvertedText,
        Timestamp,
        DateTimeUtc,
        UserId,
        SystemMessageType,
        MessageId,
        UpdatedAt,
        EditedAt,
        EditedToolTip,
        Alias,
        Avatar,
        Groupable,
        ParseUrls,
        MessageType,
        Attachments,
        Urls,
        Date,
        DateDiffersFromPrevious,
        CanEditMessage,
        CanDeleteMessage,
        Starred,
        UsernameUrl,
        Roles,
        Reactions,
        Ignored,
        Pinned,
        DiscussionCount,
        DiscussionRoomId,
        DiscussionLastMessage,
        ThreadCount,
        ThreadLastMessage,
        ThreadMessageId,
        ThreadMessagePreview,
        ThreadMessage,
        ThreadMessageFollowed,
        ShowTranslatedMessage,
        DisplayAttachment,
        DisplayLastSeenMessage,
        DisplayUrlPreview,
        Emoji,
        AvatarInfo,
        PendingMessage,
        ShowIgnoredMessage,
        MessageInEditMode,
        DirectChannels,
        HoverHighLight,
        LocalTranslation,
        OriginalMessageOrAttachmentDescription,
        MessageReplies,
        Unread,
        TextToSpeechInProgress,
        PrivateMessage,
        LastMessageRoles = PrivateMessage,
    };
    Q_ENUM(MessageRoles)

    explicit MessagesModel(const QByteArray &roomID = "no_room"_ba, RocketChatAccount *account = nullptr, Room *room = nullptr, QObject *parent = nullptr);
    ~MessagesModel() override;

    /**
     * @brief Adds a number of messages to the model
     *
     * @param messages The messages to be added
     */
    void addMessages(const QList<Message> &messages, bool insertListMessages = false);

    /**
     * @brief returns number of messages in the model
     *
     * @param parent, it is void
     * @return int, The number of messages in QList mAllMessages
     */
    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    /**
     * @brief Returns last timestamp of last message in QList mAllMessages
     *
     * @return qint64 The last timestamp
     */
    [[nodiscard]] qint64 lastTimestamp() const;

    void deleteMessage(const QByteArray &messageId);

    [[nodiscard]] qint64 generateNewStartTimeStamp(qint64 lastTimeStamp);

    void setRoomId(const QByteArray &roomId);
    [[nodiscard]] QByteArray roomId() const;

    [[nodiscard]] bool isEmpty() const;

    void clear();

    void activate();
    void deactivate();

    [[nodiscard]] Message findLastMessageBefore(const QByteArray &messageId, const std::function<bool(const Message &)> &predicate) const;
    [[nodiscard]] Message findNextMessageAfter(const QByteArray &messageId, const std::function<bool(const Message &)> &predicate) const;
    [[nodiscard]] Message findMessageById(const QByteArray &messageId) const;
    [[nodiscard]] QModelIndex indexForMessage(const QByteArray &messageId) const;

    [[nodiscard]] QByteArray messageIdFromIndex(int rowIndex);

    [[nodiscard]] QString searchText() const;
    void setSearchText(const QString &searchText);

    [[nodiscard]] Message threadMessage(const QByteArray &threadMessageId) const;

    void clearHistory();

    [[nodiscard]] qint64 firstTimestamp() const;

    void addMessagesSyncAfterLoadingFromDatabase(QList<Message> messages);

    [[nodiscard]] RuqolaQuickSearchMessageSettings *quickSearchMessageSettings() const;

    [[nodiscard]] QByteArray lastMessageId() const;

    [[nodiscard]] HighlightSearchStringIndexInMessage highlightSearchStringIndexInMessage() const;
    void setHighlightSearchStringIndexInMessage(const HighlightSearchStringIndexInMessage &newHighlightSearchStringIndexInMessage);
    void clearHighlightSearchStringIndexInMessage();

private:
    LIBRUQOLACORE_NO_EXPORT void slotFileDownloaded(const QString &filePath, const QUrl &cacheImageUrl);
    /**
     * @brief Adds a message to the model
     *
     * @param message The message to be added
     */
    LIBRUQOLACORE_NO_EXPORT void addMessage(const Message &message);

    LIBRUQOLACORE_NO_EXPORT void refresh();
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT bool threadMessageFollowed(const QByteArray &threadMessageId) const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QStringList roomRoles(const QByteArray &userId) const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString convertMessageText(const Message &message,
                                                                     const QString &userName,
                                                                     const QStringList &highlightWords,
                                                                     const QString &searchedText) const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString threadMessagePreview(const QByteArray &threadMessageId) const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QList<Message>::iterator findMessage(const QByteArray &messageId);
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QList<Message>::const_iterator findMessage(const QByteArray &messageId) const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString convertedText(const Message &message, const QString &searchedText) const;
    [[nodiscard]] bool messageReplies(const Message &message) const;

    HighlightSearchStringIndexInMessage mHighlightSearchStringIndexInMessage;
    QString mSearchText;
    QByteArray mRoomId;
    QList<Message> mAllMessages;
    RocketChatAccount *mRocketChatAccount = nullptr;
    QPointer<Room> mRoom;
    std::unique_ptr<LoadRecentHistoryManager> mLoadRecentHistoryManager;
    RuqolaQuickSearchMessageSettings *const mQuickSearchMessageSettings;
};
Q_DECLARE_METATYPE(MessagesModel::AttachmentAndUrlPreviewVisibility)
Q_DECLARE_TYPEINFO(MessagesModel::AttachmentAndUrlPreviewVisibility, Q_RELOCATABLE_TYPE);
Q_DECLARE_METATYPE(MessagesModel::HighlightSearchStringIndexInMessage)
Q_DECLARE_TYPEINFO(MessagesModel::HighlightSearchStringIndexInMessage, Q_RELOCATABLE_TYPE);
