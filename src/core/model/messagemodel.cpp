/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include <QDataStream>
#include <QFile>
#include <QJsonDocument>
#include <QModelIndex>

#include "loadrecenthistorymanager.h"
#include "messagemodel.h"
#include "rocketchataccount.h"
#include "room.h"
#include "ruqola_debug.h"
#include "ruqolaserverconfig.h"
#include "textconverter.h"
#include "texthighlighter.h"
#include "utils.h"

#include <KLocalizedString>

#include <emoticons/emojimanager.h>

// TODO reactivate when we will able to load message between cache and official server.
//#define STORE_MESSAGE 1

MessageModel::MessageModel(const QString &roomID, RocketChatAccount *account, Room *room, QObject *parent)
    : QAbstractListModel(parent)
    , mRoomId(roomID)
    , mRocketChatAccount(account)
    , mRoom(room)
{
    mLoadRecentHistoryManager = new LoadRecentHistoryManager;
    qCDebug(RUQOLA_LOG) << "Creating message Model";
#ifdef STORE_MESSAGE
    if (mRocketChatAccount) {
        const QString cachePath = mRocketChatAccount->settings()->cacheBasePath();
        if (cachePath.isEmpty()) {
            qCWarning(RUQOLA_LOG) << " Cache Path is not defined";
            return;
        }

        QDir cacheDir(cachePath + QStringLiteral("/rooms_cache"));

        // load cache
        if (QFile::exists(cacheDir.absoluteFilePath(roomID)) && !roomID.isEmpty()) {
            QFile f(cacheDir.absoluteFilePath(roomID));
            if (f.open(QIODevice::ReadOnly)) {
                auto *emojiManager = mRocketChatAccount->emojiManager();
                QDataStream in(&f);
                while (!f.atEnd()) {
                    char *byteArray;
                    quint32 length;
                    in.readBytes(byteArray, length);
                    const QByteArray arr = QByteArray::fromRawData(byteArray, length);
                    Message m = Message::fromJSon(QJsonDocument::fromBinaryData(arr).object(), emojiManager);
                    addMessage(m);
                }
            }
        }
    }
#endif
    if (mRoom) {
        connect(mRoom, &Room::rolesChanged, this, &MessageModel::refresh);
        connect(mRoom, &Room::ignoredUsersChanged, this, &MessageModel::refresh);
        connect(mRoom, &Room::highlightsWordChanged, this, &MessageModel::refresh);
    }
}

MessageModel::~MessageModel()
{
#ifdef STORE_MESSAGE
    if (mRocketChatAccount) {
        const QString cachePath = mRocketChatAccount->settings()->cacheBasePath();
        if (cachePath.isEmpty()) {
            qCWarning(RUQOLA_LOG) << " Cache Path is not defined";
            return;
        }

        QDir cacheDir(cachePath + QStringLiteral("/rooms_cache"));
        qCDebug(RUQOLA_LOG) << "Caching to..." << cacheDir.path();
        if (!cacheDir.exists(cacheDir.path())) {
            cacheDir.mkpath(cacheDir.path());
        }

        QFile f(cacheDir.absoluteFilePath(mRoomID));

        if (f.open(QIODevice::WriteOnly)) {
            QDataStream out(&f);
            for (const Message &m : std::as_const(mAllMessages)) {
                const QByteArray ms = Message::serialize(m);
                out.writeBytes(ms, ms.size());
            }
        }
    }
#endif
    delete mLoadRecentHistoryManager;
}

void MessageModel::activate()
{
    if (mRocketChatAccount) {
        connect(mRocketChatAccount, &RocketChatAccount::fileDownloaded, this, &MessageModel::slotFileDownloaded);
    }
}

void MessageModel::deactivate()
{
    if (mRocketChatAccount) {
        disconnect(mRocketChatAccount, &RocketChatAccount::fileDownloaded, this, &MessageModel::slotFileDownloaded);
    }
}

Message MessageModel::findLastMessageBefore(const QString &messageId, const std::function<bool(const Message &)> &predicate) const
{
    auto it = findMessage(messageId); // if it == end, we'll start from there
    auto rit = QVector<Message>::const_reverse_iterator(it); // this points to *it-1 already
    rit = std::find_if(rit, mAllMessages.rend(), predicate);
    return rit == mAllMessages.rend() ? Message() : *rit;
}

Message MessageModel::findNextMessageAfter(const QString &messageId, const std::function<bool(const Message &)> &predicate) const
{
    auto it = findMessage(messageId);
    if (it == mAllMessages.end()) {
        return Message(); // no wrap around, otherwise Alt+Key_Up would edit the oldest msg right away
    } else {
        ++it;
    }
    it = std::find_if(it, mAllMessages.end(), predicate);
    return it == mAllMessages.end() ? Message() : *it;
}

Message MessageModel::findMessageById(const QString &messageId) const
{
    auto it = findMessage(messageId);
    return it == mAllMessages.end() ? Message() : *it;
}

QModelIndex MessageModel::indexForMessage(const QString &messageId) const
{
    auto it = findMessage(messageId);
    if (it == mAllMessages.end()) {
        return {};
    }
    const QModelIndex idx = createIndex(std::distance(mAllMessages.begin(), it), 0);
    return idx;
}

QString MessageModel::messageIdFromIndex(int rowIndex)
{
    if (rowIndex >= 0 && rowIndex < mAllMessages.count()) {
        return mAllMessages.at(rowIndex).messageId();
    }
    return {};
}

void MessageModel::refresh()
{
    beginResetModel();
    endResetModel();
}

qint64 MessageModel::lastTimestamp() const
{
    if (!mAllMessages.isEmpty()) {
        // qCDebug(RUQOLA_LOG) << "returning timestamp" << mAllMessages.last().timeStamp();
        return mAllMessages.first().timeStamp();
    } else {
        return 0;
    }
}

int MessageModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mAllMessages.size();
}

static bool compareTimeStamps(const Message &lhs, const Message &rhs)
{
    return lhs.timeStamp() < rhs.timeStamp();
}

void MessageModel::addMessage(const Message &message)
{
    auto it = std::upper_bound(mAllMessages.begin(), mAllMessages.end(), message, compareTimeStamps);

    auto emitChanged = [this](int rowNumber) {
        const QModelIndex index = createIndex(rowNumber, 0);
        Q_EMIT dataChanged(index, index);
    };

    // When we have 1 element.
    if (mAllMessages.count() == 1 && (*mAllMessages.begin()).messageId() == message.messageId()) {
        (*mAllMessages.begin()) = message;
        qCDebug(RUQOLA_LOG) << "Update first message";
        emitChanged(0);
    } else if (((it) != mAllMessages.begin() && (*(it - 1)).messageId() == message.messageId())) {
        qCDebug(RUQOLA_LOG) << "Update message";
        if (message.pendingMessage()) {
            // If we already have a message and we must add pending message it's that server
            // send quickly new message => replace not it by a pending message
            return;
        }
        (*(it - 1)) = message;
        emitChanged(std::distance(mAllMessages.begin(), it - 1));
    } else {
        const int pos = it - mAllMessages.begin();
        beginInsertRows(QModelIndex(), pos, pos);
        mAllMessages.insert(it, message);
        endInsertRows();
    }
}

void MessageModel::addMessages(const QVector<Message> &messages, bool insertListMessages)
{
    if (messages.isEmpty()) {
        return;
    }
    if (mAllMessages.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, messages.count() - 1);
        mAllMessages = messages;
        std::sort(mAllMessages.begin(), mAllMessages.end(), compareTimeStamps);
        endInsertRows();
    } else if (insertListMessages) {
        beginResetModel();
        mAllMessages += messages;
        std::sort(mAllMessages.begin(), mAllMessages.end(), compareTimeStamps);
        endResetModel();
    } else {
        // TODO optimize this case as well?
        for (const Message &message : messages) {
            addMessage(message);
        }
    }
}

QVariant MessageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        qCWarning(RUQOLA_LOG) << "ERROR: invalid index";
        return {};
    }
    const int idx = index.row();
    const Message &message = mAllMessages.at(idx);

    switch (role) {
    case MessageModel::MessagePointer:
        return QVariant::fromValue(&message);
    case MessageModel::Username:
        return message.username();
    case MessageModel::OriginalMessage:
        return message.text();
    case MessageModel::DateTimeUtc:
        return QDateTime::fromMSecsSinceEpoch(message.timeStamp()).toUTC().toString(Qt::ISODateWithMs);
    case MessageModel::MessageConvertedText:
        return convertedText(message);
    case MessageModel::Timestamp:
        return message.displayTime();
    case MessageModel::UserId:
        return message.userId();
    case MessageModel::SystemMessageType:
        return message.systemMessageType();
    case MessageModel::MessageId:
        return message.messageId();
    case MessageModel::Alias:
        return message.alias();
    case MessageModel::MessageType:
        return message.messageType();
    case MessageModel::Avatar:
        return message.avatar();
    case MessageModel::EditedAt:
        return message.editedAt();
    case MessageModel::EditedByUserName:
        return message.editedByUsername();
    case MessageModel::EditedToolTip:
        return i18n("Edited at %1 by %2", message.editedDisplayTime(), message.editedByUsername());
    case MessageModel::Attachments: {
        QVariantList lst;
        lst.reserve(message.attachments().count());
        const auto attaches = message.attachments();
        for (const MessageAttachment &att : attaches) {
            lst.append(QVariant::fromValue(att));
        }
        return lst;
    }
    case MessageModel::Urls: {
        QVariantList lst;
        lst.reserve(message.urls().count());
        const auto urls = message.urls();
        for (const MessageUrl &url : urls) {
            lst.append(QVariant::fromValue(url));
        }
        return lst;
    }
    case MessageModel::Date: {
        const QDateTime currentDate = QDateTime::fromMSecsSinceEpoch(message.timeStamp());
        return currentDate.date().toString();
    }
    case MessageModel::DateDiffersFromPrevious:
        if (idx > 0) {
            const QDateTime currentDate = QDateTime::fromMSecsSinceEpoch(message.timeStamp());
            const Message &previousMessage = mAllMessages.at(idx - 1);
            const QDateTime previousDate = QDateTime::fromMSecsSinceEpoch(previousMessage.timeStamp());
            return currentDate.date() != previousDate.date();
        }
        return true; // show date at the top
    case MessageModel::CanEditMessage:
        return mRocketChatAccount->isMessageEditable(message); // && mRoom && mRoom->hasPermission(QStringLiteral("edit-message"));
    case MessageModel::CanDeleteMessage:
        return mRocketChatAccount->isMessageDeletable(message);
    case MessageModel::Starred:
        return message.isStarred();
    case MessageModel::UsernameUrl: {
        const QString username = message.username();
        if (username.isEmpty()) {
            return {};
        }
        return QStringLiteral("<a href=\'ruqola:/user/%1\'>@%1</a>").arg(message.username());
    }
    case MessageModel::Roles: {
        const QString str = roomRoles(message.userId()).join(QLatin1Char(','));
        return str;
    }
    case MessageModel::Reactions: {
        QVariantList lst;
        const auto reactions = message.reactions().reactions();
        lst.reserve(reactions.count());
        for (const Reaction &react : reactions) {
            // Convert reactions
            lst.append(QVariant::fromValue(react));
        }
        return lst;
    }
    case MessageModel::Ignored:
        return mRoom && mRoom->userIsIgnored(message.userId());
    case MessageModel::DirectChannels:
        return mRoom && (mRoom->channelType() == Room::RoomType::Direct);
    case MessageModel::Pinned:
        return message.messagePinned().pinned();
    case MessageModel::DiscussionCount:
        return message.discussionCount();
    case MessageModel::DiscussionRoomId:
        return message.discussionRoomId();
    case MessageModel::DiscussionLastMessage:
        return message.discussionLastMessage();
    case MessageModel::ThreadCount:
        return message.threadCount();
    case MessageModel::ThreadLastMessage:
        return message.threadLastMessage();
    case MessageModel::ThreadMessageId:
        return message.threadMessageId();
    case MessageModel::ThreadMessagePreview:
        return threadMessagePreview(message.threadMessageId());
    case MessageModel::Groupable:
        return message.groupable();
    case MessageModel::ShowTranslatedMessage:
        return message.showTranslatedMessage();
    case MessageModel::DisplayAttachment:
        return {}; // Unused.
    case MessageModel::DisplayLastSeenMessage:
        if (idx > 0) {
            if (mRoom) {
                const QDateTime currentDate = QDateTime::fromMSecsSinceEpoch(message.timeStamp());
                const QDateTime lastSeenDate = QDateTime::fromMSecsSinceEpoch(mRoom->lastSeenAt());
                // qDebug() << " lastSeeDate" << lastSeeDate;
                if (currentDate > lastSeenDate) {
                    const Message &previousMessage = mAllMessages.at(idx - 1);
                    const QDateTime previewMessageDate = QDateTime::fromMSecsSinceEpoch(previousMessage.timeStamp());
                    const bool result = (previewMessageDate <= lastSeenDate);
                    return result;
                }
            }
        }
        return false;
    case MessageModel::Emoji:
        return message.emoji();
    case MessageModel::AvatarInfo:
        return QVariant::fromValue(message.avatarInfo());
    case MessageModel::PendingMessage:
        return message.pendingMessage();
    case MessageModel::ShowIgnoredMessage:
        return message.showIgnoredMessage();
    case MessageModel::MessageInEditMode:
        return message.isEditingMode();
    }

    return {};
}

QString MessageModel::convertedText(const Message &message) const
{
    if (message.messageType() == Message::System) {
        return message.systemMessageText();
    } else {
        QStringList highlightWords;
        if (mRoom) {
            if (mRoom->channelType() != Room::RoomType::Direct) { // We can't ignore message but we can block user in direct message
                if (mRoom->userIsIgnored(message.userId()) && !message.showIgnoredMessage()) {
                    return QString(QStringLiteral("<i>") + i18n("Ignored Message") + QStringLiteral("</i>"));
                }
            }
            highlightWords = mRoom->highlightsWord();
        }
        const QString userName = mRocketChatAccount ? mRocketChatAccount->userName() : QString();
        const QStringList highlightWordsLst = mRocketChatAccount ? mRocketChatAccount->highlightWords() : highlightWords;
        return convertMessageText(message, userName, highlightWordsLst);
    }
}

bool MessageModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        qCWarning(RUQOLA_LOG) << "ERROR: invalid index";
        return false;
    }
    const int idx = index.row();
    Message &message = mAllMessages[idx];

    switch (role) {
    case MessageModel::DisplayAttachment: {
        const auto visibility = value.value<AttachmentVisibility>();
        auto attachments = message.attachments();
        for (int i = 0, total = attachments.count(); i < total; ++i) {
            const MessageAttachment att = attachments.at(i);
            if (att.attachmentId() == visibility.attachmentId) {
                MessageAttachment changeAttachment = attachments.takeAt(i);
                changeAttachment.setShowAttachment(visibility.show);
                attachments.insert(i, changeAttachment);
                break;
            }
        }
        message.setAttachments(attachments);
        Q_EMIT dataChanged(index, index);
        return true;
    }
    case MessageModel::ShowTranslatedMessage:
        message.setShowTranslatedMessage(value.toBool());
        Q_EMIT dataChanged(index, index);
        return true;
    case MessageModel::ShowIgnoredMessage:
        message.setShowIgnoredMessage(value.toBool());
        Q_EMIT dataChanged(index, index);
        return true;
    case MessageModel::MessageInEditMode:
        message.setIsEditingMode(value.toBool());
        Q_EMIT dataChanged(index, index);
        return true;
    }
    return false;
}

QStringList MessageModel::roomRoles(const QString &userId) const
{
    if (mRoom) {
        return mRoom->rolesForUserId(userId);
    }
    return {};
}

QString MessageModel::convertMessageText(const Message &message, const QString &userName, const QStringList &highlightWords) const
{
    QString messageStr = message.text();
    EmojiManager *emojiManager = nullptr;
    MessageCache *messageCache = nullptr;
    if (mRocketChatAccount) {
        emojiManager = mRocketChatAccount->emojiManager();
        messageCache = mRocketChatAccount->messageCache();
        if (mRocketChatAccount->hasAutotranslateSupport()) {
            if (message.showTranslatedMessage() && mRoom && mRoom->autoTranslate() && !mRoom->autoTranslateLanguage().isEmpty()) {
                const QString messageTranslation = message.messageTranslation().translatedStringFromLanguage(mRoom->autoTranslateLanguage());
                if (!messageTranslation.isEmpty()) {
                    messageStr = messageTranslation;
                }
            }
        }
    }

    QString needUpdateMessageId;
    return TextConverter::convertMessageText(messageStr,
                                             userName,
                                             mAllMessages,
                                             highlightWords,
                                             emojiManager,
                                             messageCache,
                                             needUpdateMessageId,
                                             message.mentions(),
                                             message.channels()
                                             /* TODO add searched text*/);
}

void MessageModel::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

bool MessageModel::isEmpty() const
{
    return mAllMessages.isEmpty();
}

void MessageModel::clear()
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mAllMessages.count() - 1);
        mAllMessages.clear();
        endRemoveRows();
    }
}

void MessageModel::changeShowOriginalMessage(const QString &messageId, bool showOriginal)
{
    Q_UNUSED(showOriginal)
    auto it = findMessage(messageId);
    if (it != mAllMessages.end()) {
        // TODO implement it
    }
}

void MessageModel::slotFileDownloaded(const QString &filePath, const QUrl &cacheImageUrl)
{
    auto matchesFilePath = [&](const QVector<MessageAttachment> &msgAttachments) {
        return std::find_if(msgAttachments.begin(),
                            msgAttachments.end(),
                            [&](const MessageAttachment &attach) {
                                // Transform link() the way RocketChatCache::downloadFile does it
                                return mRocketChatAccount->urlForLink(attach.imageUrlPreview()).path() == filePath;
                            })
            != msgAttachments.end();
    };
    auto it = std::find_if(mAllMessages.begin(), mAllMessages.end(), [&](const Message &msg) {
        if (!msg.attachments().isEmpty()) {
            return matchesFilePath(msg.attachments());
        }
        auto *emojiManager = mRocketChatAccount->emojiManager();
        const auto reactions = msg.reactions().reactions();
        for (const Reaction &reaction : reactions) {
            const QString fileName = emojiManager->customEmojiFileName(reaction.reactionName());
            if (!fileName.isEmpty() && mRocketChatAccount->urlForLink(fileName).path() == filePath) {
                return true;
            }
        }
        const Utils::AvatarInfo info = msg.avatarInfo();
        const QUrl iconUrlStr = QUrl(mRocketChatAccount->avatarUrl(info));
        if (!iconUrlStr.isEmpty()) {
            if (iconUrlStr == cacheImageUrl) {
                return true;
            }
        }
        return false;
    });
    if (it != mAllMessages.end()) {
        const QModelIndex idx = createIndex(std::distance(mAllMessages.begin(), it), 0);
        Q_EMIT dataChanged(idx, idx);
    } else {
        qCWarning(RUQOLA_LOG) << "Attachment not found:" << filePath;
    }
}

void MessageModel::deleteMessage(const QString &messageId)
{
    auto it = findMessage(messageId);
    if (it != mAllMessages.end()) {
        const int i = std::distance(mAllMessages.begin(), it);
        beginRemoveRows(QModelIndex(), i, i);
        mAllMessages.erase(it);
        endRemoveRows();
    }
}

qint64 MessageModel::generateNewStartTimeStamp(qint64 lastTimeStamp)
{
    return mLoadRecentHistoryManager->generateNewStartTimeStamp(lastTimeStamp);
}

QString MessageModel::threadMessagePreview(const QString &threadMessageId) const
{
    if (!threadMessageId.isEmpty()) {
        auto it = findMessage(threadMessageId);
        if (it != mAllMessages.cend()) {
            const QString userName = mRocketChatAccount ? mRocketChatAccount->userName() : QString();
            QString str = convertMessageText((*it), userName, mRocketChatAccount ? mRocketChatAccount->highlightWords() : QStringList());
            if (str.length() > 80) {
                str = str.left(80) + QStringLiteral("...");
            }
            return str;
        } else {
            qCDebug(RUQOLA_LOG) << "Thread message" << threadMessageId << "not found"; // could be a very old one
        }
    }
    return {};
}

QVector<Message>::iterator MessageModel::findMessage(const QString &messageId)
{
    return std::find_if(mAllMessages.begin(), mAllMessages.end(), [&](const Message &msg) {
        return msg.messageId() == messageId;
    });
}

QVector<Message>::const_iterator MessageModel::findMessage(const QString &messageId) const
{
    return std::find_if(mAllMessages.cbegin(), mAllMessages.cend(), [&](const Message &msg) {
        return msg.messageId() == messageId;
    });
}

QString MessageModel::roomId() const
{
    return mRoomId;
}
