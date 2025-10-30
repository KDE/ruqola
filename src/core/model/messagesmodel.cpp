/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include <QModelIndex>

#include <QTimeZone>

#include "emoticons/emojimanager.h"
#include "loadrecenthistorymanager.h"
#include "messagesmodel.h"
#include "quicksearch/ruqolaquicksearchmessagesettings.h"
#include "rocketchataccount.h"
#include "room.h"
#include "ruqola_messages_model_debug.h"
#include "textconverter.h"
#include "utils.h"

#include <KLocalizedString>

using namespace Qt::Literals::StringLiterals;
MessagesModel::MessagesModel(const QByteArray &roomID, RocketChatAccount *account, Room *room, QObject *parent)
    : QAbstractListModel(parent)
    , mRoomId(roomID)
    , mRocketChatAccount(account)
    , mRoom(room)
    , mLoadRecentHistoryManager(new LoadRecentHistoryManager)
    , mQuickSearchMessageSettings(new RuqolaQuickSearchMessageSettings(this, this))
{
    qCDebug(RUQOLA_MESSAGEMODELS_LOG) << "Creating message Model";
    if (mRoom) {
        connect(mRoom, &Room::rolesChanged, this, &MessagesModel::refresh);
        connect(mRoom, &Room::ignoredUsersChanged, this, &MessagesModel::refresh);
        connect(mRoom, &Room::highlightsWordChanged, this, &MessagesModel::refresh);
    }
}

MessagesModel::~MessagesModel() = default;

void MessagesModel::activate()
{
    qCDebug(RUQOLA_MESSAGEMODELS_LOG) << "Activate model";
    if (mRocketChatAccount) {
        qCDebug(RUQOLA_MESSAGEMODELS_LOG) << "CONNECT: messages model";
        connect(mRocketChatAccount, &RocketChatAccount::fileDownloaded, this, &MessagesModel::slotFileDownloaded);
    }
}

void MessagesModel::deactivate()
{
    qCDebug(RUQOLA_MESSAGEMODELS_LOG) << "Deactivate model";
    if (mRocketChatAccount) {
        qCDebug(RUQOLA_MESSAGEMODELS_LOG) << "DISCONNECT: messages model";
        disconnect(mRocketChatAccount, &RocketChatAccount::fileDownloaded, this, &MessagesModel::slotFileDownloaded);
    }
}

Message MessagesModel::findLastMessageBefore(const QByteArray &messageId, const std::function<bool(const Message &)> &predicate) const
{
    auto it = findMessage(messageId); // if it == end, we'll start from there
    auto rit = QList<Message>::const_reverse_iterator(it); // this points to *it-1 already
    rit = std::find_if(rit, mAllMessages.rend(), predicate);
    return rit == mAllMessages.rend() ? Message() : *rit;
}

Message MessagesModel::findNextMessageAfter(const QByteArray &messageId, const std::function<bool(const Message &)> &predicate) const
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

Message MessagesModel::findMessageById(const QByteArray &messageId) const
{
    auto it = findMessage(messageId);
    return it == mAllMessages.end() ? Message() : *it;
}

QModelIndex MessagesModel::indexForMessage(const QByteArray &messageId) const
{
    auto it = findMessage(messageId);
    if (it == mAllMessages.end()) {
        return {};
    }
    const QModelIndex idx = createIndex(std::distance(mAllMessages.begin(), it), 0);
    return idx;
}

QByteArray MessagesModel::messageIdFromIndex(int rowIndex)
{
    if (rowIndex >= 0 && rowIndex < mAllMessages.count()) {
        return mAllMessages.at(rowIndex).messageId();
    }
    return {};
}

QByteArray MessagesModel::lastMessageId() const
{
    if (!mAllMessages.isEmpty()) {
        // qCDebug(RUQOLA_MESSAGEMODELS_LOG) << "returning timestamp" << mAllMessages.last().timeStamp();
        return mAllMessages.at(mAllMessages.count() - 1).messageId();
    } else {
        return {};
    }
}

void MessagesModel::refresh()
{
    beginResetModel();
    endResetModel();
}

qint64 MessagesModel::lastTimestamp() const
{
    if (!mAllMessages.isEmpty()) {
        // qCDebug(RUQOLA_MESSAGEMODELS_LOG) << "returning timestamp" << mAllMessages.last().timeStamp();
        return mAllMessages.constFirst().timeStamp();
    } else {
        return 0;
    }
}

qint64 MessagesModel::firstTimestamp() const
{
    if (!mAllMessages.isEmpty()) {
        // qCDebug(RUQOLA_MESSAGEMODELS_LOG) << "returning timestamp" << mAllMessages.last().timeStamp();
        return mAllMessages.at(mAllMessages.count() - 1).timeStamp();
    } else {
        return 0;
    }
}

int MessagesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) { // flat model
        return 0;
    }
    return mAllMessages.size();
}

static bool compareTimeStamps(const Message &lhs, const Message &rhs)
{
    return lhs.timeStamp() < rhs.timeStamp();
}

void MessagesModel::addMessage(const Message &message)
{
    auto it = std::upper_bound(mAllMessages.begin(), mAllMessages.end(), message, compareTimeStamps);

    auto emitChanged = [this](int rowNumber, const QList<int> &roles = QList<int>()) {
        const QModelIndex index = createIndex(rowNumber, 0);
        Q_EMIT dataChanged(index, index, roles);
    };

    // When we have 1 element.
    if (mAllMessages.count() == 1 && (*mAllMessages.begin()).messageId() == message.messageId()) {
        (*mAllMessages.begin()) = message;
        qCDebug(RUQOLA_MESSAGEMODELS_LOG) << "Update first message";
        emitChanged(0);
    } else if (((it) != mAllMessages.begin() && (*(it - 1)).messageId() == message.messageId())) {
        qCDebug(RUQOLA_MESSAGEMODELS_LOG) << "Update message: " << message.text();
        if (message.pendingMessage()) {
            // If we already have a message and we must add pending message it's that server
            // send quickly new message => replace not it by a pending message
            return;
        }
        (*(it - 1)) = message;
        emitChanged(std::distance(mAllMessages.begin(), it - 1), {OriginalMessageOrAttachmentDescription});
    } else {
        qCDebug(RUQOLA_MESSAGEMODELS_LOG) << "Add message: " << message.text();
        const int pos = it - mAllMessages.begin();
        beginInsertRows(QModelIndex(), pos, pos);
        mAllMessages.insert(it, message);
        endInsertRows();
    }
}

void MessagesModel::addMessagesSyncAfterLoadingFromDatabase(QList<Message> messages)
{
    if (messages.count() > 50) {
        beginResetModel();
        std::sort(messages.begin(), messages.end(), compareTimeStamps);
        const QList<Message> reducedMessageList = messages.mid(messages.count() - 50);
        mAllMessages = reducedMessageList;
        endResetModel();
    } else {
        // TODO optimize this case as well?
        for (const Message &message : messages) {
            addMessage(message);
        }
    }
}

void MessagesModel::addMessages(const QList<Message> &messages, bool insertListMessages)
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

QVariant MessagesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        qCWarning(RUQOLA_MESSAGEMODELS_LOG) << "ERROR: invalid index";
        return {};
    }
    const int idx = index.row();
    const Message &message = mAllMessages.at(idx);

    switch (role) {
    case MessagesModel::MessagePointer:
        return QVariant::fromValue(&message);
    case MessagesModel::Username:
        return message.username();
    case Qt::AccessibleTextRole:
    case MessagesModel::OriginalMessage:
        return message.text();
    case MessagesModel::DateTimeUtc:
        return QDateTime::fromMSecsSinceEpoch(message.timeStamp(), QTimeZone::UTC).toString(Qt::ISODateWithMs);
    case MessagesModel::MessageConvertedText:
        return convertedText(message, mSearchText);
    case MessagesModel::Timestamp:
        return message.displayTime();
    case MessagesModel::UserId:
        return message.userId();
    case MessagesModel::SystemMessageType:
        return QVariant::fromValue<SystemMessageTypeUtil::SystemMessageType>(message.systemMessageType());
    case MessagesModel::MessageId:
        return message.messageId();
    case MessagesModel::Alias:
        return message.alias();
    case MessagesModel::MessageType:
        return message.messageType();
    case MessagesModel::Avatar:
        return message.avatar();
    case MessagesModel::EditedAt:
        return message.editedAt();
    case MessagesModel::EditedToolTip: {
        const QLocale l;
        const QString editedDisplayTime = l.toString(QDateTime::fromMSecsSinceEpoch(message.editedAt()), QLocale::LongFormat);
        return i18n("Edited at %1 by %2", editedDisplayTime, message.editedByUsername());
    }
    case MessagesModel::Attachments: {
        QVariantList lst;
        if (message.attachments()) {
            lst.reserve(message.attachments()->messageAttachments().count());
            const auto attaches = message.attachments()->messageAttachments();
            for (const MessageAttachment &att : attaches) {
                lst.append(QVariant::fromValue(att));
            }
        }
        return lst;
    }
    case MessagesModel::Urls: {
        QVariantList lst;
        if (message.urls()) {
            lst.reserve(message.urls()->messageUrls().count());
            const auto urls = message.urls()->messageUrls();
            for (const MessageUrl &url : urls) {
                lst.append(QVariant::fromValue(url));
            }
        }
        return lst;
    }
    case MessagesModel::Date: {
        const QDateTime currentDate = QDateTime::fromMSecsSinceEpoch(message.timeStamp());
        return QLocale().toString(currentDate.date());
    }
    case MessagesModel::DateDiffersFromPrevious:
        if (idx > 0) {
            const QDateTime currentDate = QDateTime::fromMSecsSinceEpoch(message.timeStamp(), QTimeZone::systemTimeZone());
            const Message &previousMessage = mAllMessages.at(idx - 1);
            const QDateTime previousDate = QDateTime::fromMSecsSinceEpoch(previousMessage.timeStamp(), QTimeZone::systemTimeZone());
            return currentDate.date() != previousDate.date();
        }
        return true; // show date at the top
    case MessagesModel::CanEditMessage:
        return mRocketChatAccount && mRocketChatAccount->isMessageEditable(message); // && mRoom && mRoom->hasPermission(u"edit-message"_s);
    case MessagesModel::CanDeleteMessage:
        return mRocketChatAccount && mRocketChatAccount->isMessageDeletable(message);
    case MessagesModel::Starred:
        return message.isStarred();
    case MessagesModel::UsernameUrl: {
        const QString username = message.username();
        if (username.isEmpty()) {
            return {};
        }
        return u"<a href=\'ruqola:/user/%1\'>@%1</a>"_s.arg(message.username());
    }
    case MessagesModel::Roles: {
        const QString str = roomRoles(message.userId()).join(u',');
        return str;
    }
    case MessagesModel::Reactions: {
        QVariantList lst;
        if (auto reactionsMessages = message.reactions()) {
            const auto reactions = reactionsMessages->reactions();
            lst.reserve(reactions.count());
            for (const Reaction &react : reactions) {
                // Convert reactions
                lst.append(QVariant::fromValue(react));
            }
        }
        return lst;
    }
    case MessagesModel::Ignored:
        return mRoom && mRoom->userIsIgnored(message.userId());
    case MessagesModel::DirectChannels:
        return mRoom && (mRoom->channelType() == Room::RoomType::Direct);
    case MessagesModel::Pinned:
        return message.messagePinned() && message.messagePinned()->pinned();
    case MessagesModel::DiscussionCount:
        return message.discussionCount();
    case MessagesModel::DiscussionRoomId:
        return message.discussionRoomId();
    case MessagesModel::DiscussionLastMessage:
        return message.discussionLastMessage();
    case MessagesModel::ThreadCount:
        return message.threadCount();
    case MessagesModel::ThreadLastMessage:
        return message.threadLastMessage();
    case MessagesModel::ThreadMessageId:
        return message.threadMessageId();
    case MessagesModel::ThreadMessagePreview:
        return threadMessagePreview(message.threadMessageId());
    case MessagesModel::ThreadMessageFollowed:
        return threadMessageFollowed(message.threadMessageId());
    case MessagesModel::ThreadMessage: {
        const Message tm = threadMessage(message.threadMessageId());
        return QVariant::fromValue(tm);
    }
    case MessagesModel::Groupable:
        return message.groupable();
    case MessagesModel::ShowTranslatedMessage:
        return message.showTranslatedMessage();
    case MessagesModel::DisplayAttachment:
    case MessagesModel::DisplayUrlPreview:
        return {}; // Unused.
    case MessagesModel::DisplayLastSeenMessage:
        if (idx > 0) {
            if (mRoom) {
                const QDateTime currentDate = QDateTime::fromMSecsSinceEpoch(message.timeStamp(), QTimeZone::utc());
                const QDateTime lastSeenDate = QDateTime::fromMSecsSinceEpoch(mRoom->lastSeenAt(), QTimeZone::utc());
                // qDebug() << " lastSeeDate" << lastSeeDate;
                if (currentDate > lastSeenDate) {
                    const Message &previousMessage = mAllMessages.at(idx - 1);
                    const QDateTime previewMessageDate = QDateTime::fromMSecsSinceEpoch(previousMessage.timeStamp(), QTimeZone::utc());
                    const bool result = (previewMessageDate <= lastSeenDate);
                    return result;
                }
            }
        }
        return false;
    case MessagesModel::Emoji:
        return message.emoji();
    case MessagesModel::AvatarInfo:
        return QVariant::fromValue(message.avatarInfo());
    case MessagesModel::PendingMessage:
        return message.pendingMessage();
    case MessagesModel::ShowIgnoredMessage:
        return message.showIgnoredMessage();
    case MessagesModel::MessageInEditMode:
        return message.isEditingMode();
    case MessagesModel::HoverHighLight:
        return message.hoverHighlight();
    case MessagesModel::LocalTranslation:
        return message.localTranslation();
    case MessagesModel::OriginalMessageOrAttachmentDescription:
        return message.originalMessageOrAttachmentDescription();
    case MessagesModel::PrivateMessage:
        return message.privateMessage();
    case MessagesModel::MessageReplies:
        return messageReplies(message);
    case MessagesModel::Unread:
        return message.unread();
    case MessagesModel::TextToSpeechInProgress:
        return message.textToSpeechInProgress();
    default:
        break;
    }

    return {};
}

QString MessagesModel::convertedText(const Message &message, const QString &searchedText) const
{
    if (message.messageType() == Message::System) {
        return message.systemMessageText();
    } else if (message.messageType() == Message::EncryptedText) {
        // TODO allow to decrypt message
        return message.systemMessageText();
    } else {
        QStringList highlightWords;
        if (mRoom) {
            if (mRoom->channelType() != Room::RoomType::Direct) { // We can't ignore message but we can block user in direct message
                if (mRoom->userIsIgnored(message.userId()) && !message.showIgnoredMessage()) {
                    return QString(u"<i>"_s + i18n("Ignored Message") + u"</i>"_s);
                }
            }
            highlightWords = mRoom->highlightsWord();
        }
        const QString userName = mRocketChatAccount ? mRocketChatAccount->userName() : QString();
        const QStringList highlightWordsLst = mRocketChatAccount ? mRocketChatAccount->highlightWords() : highlightWords;
        const QString convertedMessage{convertMessageText(message, userName, highlightWordsLst, searchedText)};
        if (message.privateMessage()) {
            return i18n("Only you can see this message") + convertedMessage;
        }
        return convertedMessage;
    }
}

bool MessagesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        qCWarning(RUQOLA_MESSAGEMODELS_LOG) << "ERROR: invalid index";
        return false;
    }
    const int idx = index.row();
    Message &message = mAllMessages[idx];

    switch (role) {
    case MessagesModel::DisplayAttachment: {
        const auto visibility = value.value<AttachmentAndUrlPreviewVisibility>();
        if (message.attachments()) {
            auto attachments = message.attachments()->messageAttachments();
            for (int i = 0, total = attachments.count(); i < total; ++i) {
                const MessageAttachment att = attachments.at(i);
                if (att.attachmentId() == visibility.elementId) {
                    MessageAttachment changeAttachment = attachments.takeAt(i);
                    changeAttachment.setShowAttachment(visibility.show);
                    attachments.insert(i, changeAttachment);
                    break;
                }
            }
            MessageAttachments d;
            d.setMessageAttachments(attachments);
            message.setAttachments(d);
            Q_EMIT dataChanged(index, index, {MessagesModel::DisplayAttachment});
            return true;
        } else {
            return false;
        }
    }
    case MessagesModel::DisplayUrlPreview: {
        const auto visibility = value.value<AttachmentAndUrlPreviewVisibility>();
        if (message.urls()) {
            auto urls = message.urls()->messageUrls();
            for (int i = 0, total = urls.count(); i < total; ++i) {
                const MessageUrl att = urls.at(i);
                if (att.urlId() == visibility.elementId) {
                    MessageUrl changeUrlPreview = urls.takeAt(i);
                    changeUrlPreview.setShowPreview(visibility.show);
                    urls.insert(i, changeUrlPreview);
                    break;
                }
            }
            MessageUrls d;
            d.setMessageUrls(urls);
            message.setUrls(d);
            Q_EMIT dataChanged(index, index, {MessagesModel::DisplayUrlPreview});
            return true;
        } else {
            return false;
        }
    }
    case MessagesModel::ShowTranslatedMessage:
        message.setShowTranslatedMessage(value.toBool());
        Q_EMIT dataChanged(index, index, {MessagesModel::ShowTranslatedMessage});
        return true;
    case MessagesModel::ShowIgnoredMessage:
        message.setShowIgnoredMessage(value.toBool());
        Q_EMIT dataChanged(index, index, {MessagesModel::ShowIgnoredMessage});
        return true;
    case MessagesModel::MessageInEditMode:
        message.setIsEditingMode(value.toBool());
        Q_EMIT dataChanged(index, index, {MessagesModel::MessageInEditMode});
        return true;
    case MessagesModel::HoverHighLight:
        message.setHoverHighlight(value.toBool());
        Q_EMIT dataChanged(index, index, {MessagesModel::HoverHighLight});
        return true;
    case MessagesModel::TextToSpeechInProgress:
        message.setTextToSpeechInProgress(value.toBool());
        Q_EMIT dataChanged(index, index, {MessagesModel::TextToSpeechInProgress});
        return true;
    case MessagesModel::LocalTranslation:
        message.setLocalTranslation(value.toString());
        // Make sure that we show translated message
        message.setShowTranslatedMessage(true);
        Q_EMIT dataChanged(index, index, {MessagesModel::ShowTranslatedMessage, MessagesModel::LocalTranslation});
        return true;
    }
    return false;
}

QStringList MessagesModel::roomRoles(const QByteArray &userId) const
{
    if (mRoom) {
        return mRoom->rolesForUserId(userId);
    }
    return {};
}

QString MessagesModel::convertMessageText(const Message &message, const QString &userName, const QStringList &highlightWords, const QString &searchedText) const
{
    QString messageStr = message.text();
    EmojiManager *emojiManager = nullptr;
    MessageCache *messageCache = nullptr;
    int maximumRecursiveQuotedText = -1;
    if (mRocketChatAccount) {
        emojiManager = mRocketChatAccount->emojiManager();
        messageCache = mRocketChatAccount->messageCache();
        maximumRecursiveQuotedText = mRocketChatAccount->ruqolaServerConfig()->messageQuoteChainLimit();
        if (mRocketChatAccount->hasAutotranslateSupport()) {
            if (message.showTranslatedMessage()) {
                if (mRoom && mRoom->autoTranslate() && !mRoom->autoTranslateLanguage().isEmpty()) {
                    QString messageTranslation;
                    if (message.messageTranslation()) {
                        messageTranslation = message.messageTranslation()->translatedStringFromLanguage(mRoom->autoTranslateLanguage());
                    }
                    if (!messageTranslation.isEmpty()) {
                        messageStr = messageTranslation;
                    } else if (!message.localTranslation().isEmpty()) {
                        messageStr = message.localTranslation();
                    }
                }
            }
        } else if (message.showTranslatedMessage() && !message.localTranslation().isEmpty()) {
            messageStr = message.localTranslation();
        }
    }

    QByteArray needUpdateMessageId;
    const TextConverter::ConvertMessageTextSettings settings(messageStr,
                                                             userName,
                                                             mAllMessages,
                                                             highlightWords,
                                                             emojiManager,
                                                             messageCache,
                                                             message.mentions(),
                                                             message.channels(),
                                                             message.messageId(),
                                                             searchedText,
                                                             maximumRecursiveQuotedText);

    int recursiveIndex = 0;
    int hightLightStringIndex = 0;
    if (message.messageId() == mHighlightSearchStringIndexInMessage.messageId) {
        hightLightStringIndex = mHighlightSearchStringIndexInMessage.index;
    }
    int numberOfTextSearched = 0;
    return TextConverter::convertMessageText(settings, needUpdateMessageId, recursiveIndex, numberOfTextSearched, hightLightStringIndex);
}

void MessagesModel::setRoomId(const QByteArray &roomId)
{
    mRoomId = roomId;
}

bool MessagesModel::isEmpty() const
{
    return mAllMessages.isEmpty();
}

void MessagesModel::clear()
{
    mSearchText.clear();
    if (rowCount() != 0) {
        beginResetModel();
        mAllMessages.clear();
        endResetModel();
    }
}

void MessagesModel::slotFileDownloaded(const QString &filePath, const QUrl &cacheImageUrl)
{
    auto matchesFilePath = [&](const QList<MessageAttachment> &msgAttachments) {
        return std::find_if(msgAttachments.begin(),
                            msgAttachments.end(),
                            [&](const MessageAttachment &attach) {
                                // Transform link() the way RocketChatCache::downloadFile does it
                                return mRocketChatAccount->urlForLink(attach.imageUrlPreview()).path() == filePath;
                            })
            != msgAttachments.end();
    };
    auto it = std::find_if(mAllMessages.begin(), mAllMessages.end(), [&](const Message &msg) {
        if (msg.attachments() && !msg.attachments()->messageAttachments().isEmpty()) {
            return matchesFilePath(msg.attachments()->messageAttachments());
        }
        auto *emojiManager = mRocketChatAccount->emojiManager();
        if (auto reactionsMessages = msg.reactions()) {
            const auto reactions = reactionsMessages->reactions();
            for (const Reaction &reaction : reactions) {
                const QString fileName = emojiManager->customEmojiFileName(reaction.reactionName());
                if (!fileName.isEmpty() && mRocketChatAccount->urlForLink(fileName).path() == filePath) {
                    return true;
                }
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
        // Not necessarily a problem. The signal is emitted for CustomSounds or avatars, not just for attachments.
        // qCDebug(RUQOLA_MESSAGEMODELS_LOG) << "Attachment not found:" << filePath << "in" << mRoom->name() << "which has" << mAllMessages.count() <<
        // "messages";
    }
}

void MessagesModel::deleteMessage(const QByteArray &messageId)
{
    auto it = findMessage(messageId);
    if (it != mAllMessages.end()) {
        const int i = std::distance(mAllMessages.begin(), it);
        beginRemoveRows(QModelIndex(), i, i);
        mAllMessages.erase(it);
        endRemoveRows();
    }
}

qint64 MessagesModel::generateNewStartTimeStamp(qint64 lastTimeStamp)
{
    return mLoadRecentHistoryManager->generateNewStartTimeStamp(lastTimeStamp);
}

Message MessagesModel::threadMessage(const QByteArray &threadMessageId) const
{
    if (!threadMessageId.isEmpty()) {
        auto it = findMessage(threadMessageId);
        if (it != mAllMessages.cend()) {
            return (*it);
        } else {
            qCDebug(RUQOLA_MESSAGEMODELS_LOG) << "Thread message" << threadMessageId << "not found"; // could be a very old one
        }
    }
    return Message{};
}

QString MessagesModel::threadMessagePreview(const QByteArray &threadMessageId) const
{
    if (!threadMessageId.isEmpty()) {
        auto it = findMessage(threadMessageId);
        if (it != mAllMessages.cend()) {
            const QString userName = mRocketChatAccount ? mRocketChatAccount->userName() : QString();
            QString str = convertMessageText((*it), userName, mRocketChatAccount ? mRocketChatAccount->highlightWords() : QStringList(), QString());
            if (str.length() > 80) {
                str = str.left(80) + u"..."_s;
            }
            return str;
        } else {
            qCDebug(RUQOLA_MESSAGEMODELS_LOG) << "Thread message" << threadMessageId << "not found"; // could be a very old one
        }
    }
    return {};
}

bool MessagesModel::messageReplies(const Message &message) const
{
    if (mRocketChatAccount) {
        return message.replies() && message.replies()->replies().contains(mRocketChatAccount->userId());
    }
    return false;
}

MessagesModel::HighlightSearchStringIndexInMessage MessagesModel::highlightSearchStringIndexInMessage() const
{
    return mHighlightSearchStringIndexInMessage;
}

void MessagesModel::setHighlightSearchStringIndexInMessage(const HighlightSearchStringIndexInMessage &newHighlightSearchStringIndexInMessage)
{
    if (mHighlightSearchStringIndexInMessage != newHighlightSearchStringIndexInMessage) {
        mHighlightSearchStringIndexInMessage = newHighlightSearchStringIndexInMessage;
        auto it = findMessage(mHighlightSearchStringIndexInMessage.messageId);
        if (it != mAllMessages.cend()) {
            const QModelIndex index = indexForMessage(mHighlightSearchStringIndexInMessage.messageId);
            Q_EMIT dataChanged(index, index);
        }
    }
}

void MessagesModel::clearHighlightSearchStringIndexInMessage()
{
    auto it = findMessage(mHighlightSearchStringIndexInMessage.messageId);
    if (it != mAllMessages.cend()) {
        const QModelIndex index = indexForMessage(mHighlightSearchStringIndexInMessage.messageId);
        Q_EMIT dataChanged(index, index);
    }
    mHighlightSearchStringIndexInMessage.clear();
}

RuqolaQuickSearchMessageSettings *MessagesModel::quickSearchMessageSettings() const
{
    return mQuickSearchMessageSettings;
}

bool MessagesModel::threadMessageFollowed(const QByteArray &threadMessageId) const
{
    if (!threadMessageId.isEmpty()) {
        auto it = findMessage(threadMessageId);
        if (it != mAllMessages.cend()) {
            if ((*it).replies()) {
                const QByteArray userId = mRocketChatAccount ? mRocketChatAccount->userId() : QByteArray();
                if (!userId.isEmpty()) {
                    return (*it).replies()->replies().contains(userId);
                }
            }
        } else {
            qCDebug(RUQOLA_MESSAGEMODELS_LOG) << "Thread message" << threadMessageId << "not found"; // could be a very old one
        }
    }
    return false;
}

QList<Message>::iterator MessagesModel::findMessage(const QByteArray &messageId)
{
    return std::find_if(mAllMessages.begin(), mAllMessages.end(), [&](const Message &msg) {
        return msg.messageId() == messageId;
    });
}

QList<Message>::const_iterator MessagesModel::findMessage(const QByteArray &messageId) const
{
    return std::find_if(mAllMessages.cbegin(), mAllMessages.cend(), [&](const Message &msg) {
        return msg.messageId() == messageId;
    });
}

QByteArray MessagesModel::roomId() const
{
    return mRoomId;
}

QString MessagesModel::searchText() const
{
    return mSearchText;
}

void MessagesModel::setSearchText(const QString &searchText)
{
    if (mSearchText != searchText) {
        beginResetModel();
        mSearchText = searchText;
        endResetModel();
    }
}

void MessagesModel::clearHistory()
{
    if (rowCount() != 0) {
        const auto elementSize = (mAllMessages.size() - 50);
        if (elementSize > 0) {
            beginResetModel();
            mAllMessages.remove(0, elementSize);
            endResetModel();
        }
    }
}

void MessagesModel::HighlightSearchStringIndexInMessage::clear()
{
    index = -1;
    messageId.clear();
}

bool MessagesModel::HighlightSearchStringIndexInMessage::operator==(const HighlightSearchStringIndexInMessage &other) const
{
    return index == other.index && messageId == other.messageId;
}

#include "moc_messagesmodel.cpp"
