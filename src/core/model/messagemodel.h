/*

 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include "libruqolacore_export.h"
#include "messages/message.h"
#include <QAbstractListModel>
#include <QByteArray>
#include <QObject>

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
        UserId,
        SystemMessageType,
        MessageId,
        RoomId,
        UpdatedAt,
        EditedAt,
        EditedByUserName,
        EditedByUserId,
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
        ShowTranslatedMessage,
        DisplayAttachment,
        DisplayLastSeenMessage,
        Emoji,
        AvatarInfo,
        PendingMessage,
        ShowIgnoredMessage,
        MessageInEditMode,
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
    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    /**
     * @brief Returns last timestamp of last message in QVector mAllMessages
     *
     * @return qint64 The last timestamp
     */
    Q_REQUIRED_RESULT qint64 lastTimestamp() const;

    void deleteMessage(const QString &messageId);

    Q_REQUIRED_RESULT qint64 generateNewStartTimeStamp(qint64 lastTimeStamp);

    void setRoomId(const QString &roomID);
    Q_REQUIRED_RESULT bool isEmpty() const;

    void clear();

    void changeShowOriginalMessage(const QString &messageId, bool showOriginal);

    Q_REQUIRED_RESULT QString roomId() const;

    void activate();
    void deactivate();

    Q_REQUIRED_RESULT Message findLastMessageBefore(const QString &messageId, const std::function<bool(const Message &)> &predicate) const;
    Q_REQUIRED_RESULT Message findNextMessageAfter(const QString &messageId, const std::function<bool(const Message &)> &predicate) const;
    Q_REQUIRED_RESULT Message findMessageById(const QString &messageId) const;
    Q_REQUIRED_RESULT QModelIndex indexForMessage(const QString &messageId) const;

private Q_SLOTS:
    void slotFileDownloaded(const QString &filePath, const QUrl &cacheImageUrl);

private:
    Q_DISABLE_COPY(MessageModel)
    /**
     * @brief Adds a message to the model
     *
     * @param message The message to be added
     */
    void addMessage(const Message &message);

    void refresh();
    Q_REQUIRED_RESULT QStringList roomRoles(const QString &userId) const;
    Q_REQUIRED_RESULT QString convertMessageText(const Message &message, const QString &userName, const QStringList &highlightWords) const;
    Q_REQUIRED_RESULT QString threadMessagePreview(const QString &threadMessageId) const;
    Q_REQUIRED_RESULT QVector<Message>::iterator findMessage(const QString &messageId);
    Q_REQUIRED_RESULT QVector<Message>::const_iterator findMessage(const QString &messageId) const;
    Q_REQUIRED_RESULT QString convertedText(const Message &message) const;

    QString mRoomId;
    QVector<Message> mAllMessages;
    RocketChatAccount *mRocketChatAccount = nullptr;
    Room *mRoom = nullptr;
    LoadRecentHistoryManager *mLoadRecentHistoryManager = nullptr;
};
Q_DECLARE_METATYPE(MessageModel::AttachmentVisibility)
#endif
