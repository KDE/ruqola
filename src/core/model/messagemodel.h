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
class QObject;
#include <QPointer>

class RocketChatAccount;
class LoadRecentHistoryManager;
class Room;
class LIBRUQOLACORE_EXPORT MessageModel : public QAbstractListModel
{
    Q_OBJECT
public:
    struct LIBRUQOLACORE_EXPORT AttachmentVisibility {
        bool show = false;
        QString attachmentId;
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
        RoomId,
        UpdatedAt,
        EditedAt,
        EditedByUserName,
        EditedByUserId,
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
        Emoji,
        AvatarInfo,
        PendingMessage,
        ShowIgnoredMessage,
        MessageInEditMode,
        DirectChannels,
        HoverHighLight,
        LocalTranslation,
        OriginalMessageOrAttachmentDescription,
        GoToMessageBackgroundColor,
    };
    Q_ENUM(MessageRoles)

    explicit MessageModel(const QString &roomID = QStringLiteral("no_room"),
                          RocketChatAccount *account = nullptr,
                          Room *room = nullptr,
                          QObject *parent = nullptr);
    ~MessageModel() override;

    /**
     * @brief Adds a number of messages to the model
     *
     * @param messages The messages to be added
     */
    void addMessages(const QVector<Message> &messages, bool insertListMessages = false);

    /**
     * @brief returns number of messages in the model
     *
     * @param parent, it is void
     * @return int, The number of messages in QVector mAllMessages
     */
    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    /**
     * @brief Returns last timestamp of last message in QVector mAllMessages
     *
     * @return qint64 The last timestamp
     */
    [[nodiscard]] qint64 lastTimestamp() const;

    void deleteMessage(const QString &messageId);

    [[nodiscard]] qint64 generateNewStartTimeStamp(qint64 lastTimeStamp);

    void setRoomId(const QString &roomId);
    [[nodiscard]] bool isEmpty() const;

    void clear();

    void changeShowOriginalMessage(const QString &messageId, bool showOriginal);

    [[nodiscard]] QString roomId() const;

    void activate();
    void deactivate();

    [[nodiscard]] Message findLastMessageBefore(const QString &messageId, const std::function<bool(const Message &)> &predicate) const;
    [[nodiscard]] Message findNextMessageAfter(const QString &messageId, const std::function<bool(const Message &)> &predicate) const;
    [[nodiscard]] Message findMessageById(const QString &messageId) const;
    [[nodiscard]] QModelIndex indexForMessage(const QString &messageId) const;

    [[nodiscard]] QString messageIdFromIndex(int rowIndex);

    [[nodiscard]] QString searchText() const;
    void setSearchText(const QString &searchText);

    [[nodiscard]] Message threadMessage(const QString &threadMessageId) const;

private:
    LIBRUQOLACORE_NO_EXPORT void slotFileDownloaded(const QString &filePath, const QUrl &cacheImageUrl);
    Q_DISABLE_COPY(MessageModel)
    /**
     * @brief Adds a message to the model
     *
     * @param message The message to be added
     */
    LIBRUQOLACORE_NO_EXPORT void addMessage(const Message &message);

    LIBRUQOLACORE_NO_EXPORT void refresh();
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT bool threadMessageFollowed(const QString &threadMessageId) const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QStringList roomRoles(const QString &userId) const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString convertMessageText(const Message &message,
                                                                     const QString &userName,
                                                                     const QStringList &highlightWords,
                                                                     const QString &searchedText) const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString threadMessagePreview(const QString &threadMessageId) const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QVector<Message>::iterator findMessage(const QString &messageId);
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QVector<Message>::const_iterator findMessage(const QString &messageId) const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString convertedText(const Message &message, const QString &searchedText) const;

    QString mSearchText;
    QString mRoomId;
    QVector<Message> mAllMessages;
    RocketChatAccount *mRocketChatAccount = nullptr;
    QPointer<Room> mRoom;
    std::unique_ptr<LoadRecentHistoryManager> mLoadRecentHistoryManager;
};
Q_DECLARE_METATYPE(MessageModel::AttachmentVisibility)
