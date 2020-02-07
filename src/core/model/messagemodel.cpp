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

#include <QFile>
#include <QJsonDocument>
#include <QDataStream>
#include <QModelIndex>
#include <QAbstractListModel>

#include "messagemodel.h"
#include "ruqolaserverconfig.h"
#include "room.h"
#include "ruqola_debug.h"
#include "utils.h"
#include "rocketchataccount.h"
#include "texthighlighter.h"
#include "textconverter.h"
#include "loadrecenthistorymanager.h"

#include <KLocalizedString>

//TODO reactivate when we will able to load message between cache and official server.
//#define STORE_MESSAGE 1

MessageModel::MessageModel(const QString &roomID, RocketChatAccount *account, Room *room, QObject *parent)
    : QAbstractListModel(parent)
    , mRoomId(roomID)
    , mRocketChatAccount(account)
    , mRoom(room)
{
    mTextConverter = new TextConverter(mRocketChatAccount ? mRocketChatAccount->emojiManager() : nullptr);
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
                QDataStream in(&f);
                while (!f.atEnd()) {
                    char *byteArray;
                    quint32 length;
                    in.readBytes(byteArray, length);
                    const QByteArray arr = QByteArray::fromRawData(byteArray, length);
                    Message m = Message::fromJSon(QJsonDocument::fromBinaryData(arr).object());
                    addMessage(m);
                }
            }
        }
    }
#endif
    if (mRoom) {
        connect(mRoom, &Room::rolesChanged, this, &MessageModel::refresh);
        connect(mRoom, &Room::ignoredUsersChanged, this, &MessageModel::refresh);
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
            for (const Message &m : qAsConst(mAllMessages)) {
                const QByteArray ms = Message::serialize(m);
                out.writeBytes(ms, ms.size());
            }
        }
    }
#endif
    delete mTextConverter;
    delete mLoadRecentHistoryManager;
}

void MessageModel::enableQmlHacks(bool qmlHacks)
{
    mQmlHacks = qmlHacks;
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

void MessageModel::refresh()
{
    beginResetModel();
    endResetModel();
}

QHash<int, QByteArray> MessageModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[OriginalMessage] = QByteArrayLiteral("originalMessage");
    roles[MessageConvertedText] = QByteArrayLiteral("messageConverted");
    roles[Username] = QByteArrayLiteral("username");
    roles[Timestamp] = QByteArrayLiteral("timestamp");
    roles[UserId] = QByteArrayLiteral("userID");
    roles[SystemMessageType] = QByteArrayLiteral("type");
    roles[MessageId] = QByteArrayLiteral("messageID");
    roles[RoomId] = QByteArrayLiteral("roomID");
    roles[UpdatedAt] = QByteArrayLiteral("updatedAt");
    roles[EditedAt] = QByteArrayLiteral("editedAt");
    roles[EditedByUserName] = QByteArrayLiteral("editedByUsername");
    roles[EditedByUserId] = QByteArrayLiteral("editedByUserID");
    roles[Alias] = QByteArrayLiteral("alias");
    roles[Avatar] = QByteArrayLiteral("avatar");
    roles[Groupable] = QByteArrayLiteral("groupable");
    roles[MessageType] = QByteArrayLiteral("messagetype");
    roles[Attachments] = QByteArrayLiteral("attachments");
    roles[Urls] = QByteArrayLiteral("urls");
    roles[Date] = QByteArrayLiteral("date");
    roles[CanEditMessage] = QByteArrayLiteral("canEditMessage");
    roles[Starred] = QByteArrayLiteral("starred");
    roles[UsernameUrl] = QByteArrayLiteral("usernameurl");
    roles[Roles] = QByteArrayLiteral("roles");
    roles[Reactions] = QByteArrayLiteral("reactions");
    roles[Ignored] = QByteArrayLiteral("userIsIgnored");
    roles[Pinned] = QByteArrayLiteral("pinned");
    roles[DiscussionCount] = QByteArrayLiteral("discussionCount");
    roles[DiscussionRoomId] = QByteArrayLiteral("discussionRoomId");
    roles[DiscussionLastMessage] = QByteArrayLiteral("discussionLastMessage");
    roles[ThreadCount] = QByteArrayLiteral("threadCount");
    roles[ThreadLastMessage] = QByteArrayLiteral("threadLastMessage");
    roles[ThreadMessageId] = QByteArrayLiteral("threadMessageId");
    roles[ThreadMessagePreview] = QByteArrayLiteral("threadMessagePreview");
    roles[ShowTranslatedMessage] = QByteArrayLiteral("showTranslatedMessage");
    roles[DisplayAttachment] = QByteArrayLiteral("displayAttachment");
    return roles;
}

qint64 MessageModel::lastTimestamp() const
{
    if (!mAllMessages.isEmpty()) {
        //qCDebug(RUQOLA_LOG) << "returning timestamp" << mAllMessages.last().timeStamp();
        return mAllMessages.first().timeStamp();
    } else {
        return 0;
    }
}

int MessageModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mAllMessages.size();
}

static bool compareTimeStamps(const Message &lhs, const Message &rhs) {
    return lhs.timeStamp() < rhs.timeStamp();
}

void MessageModel::addMessage(const Message &message)
{
    auto it = std::upper_bound(mAllMessages.begin(), mAllMessages.end(), message, compareTimeStamps);

    auto emitChanged = [this](int rowNumber) {
                           if (mQmlHacks) {
                               //For the moment !!!! It's not optimal but Q_EMIT dataChanged(index, index); doesn't work
                               beginRemoveRows(QModelIndex(), rowNumber, rowNumber);
                               endRemoveRows();

                               beginInsertRows(QModelIndex(), rowNumber, rowNumber);
                               endInsertRows();
                           } else {
                               const QModelIndex index = createIndex(rowNumber, 0);
                               Q_EMIT dataChanged(index, index);
                           }
                       };

    //When we have 1 element.
    if (mAllMessages.count() == 1 && (*mAllMessages.begin()).messageId() == message.messageId()) {
        (*mAllMessages.begin()) = message;
        qCDebug(RUQOLA_LOG) << "Update first message";
        emitChanged(0);
    } else if (((it) != mAllMessages.begin() && (*(it - 1)).messageId() == message.messageId())) {
        qCDebug(RUQOLA_LOG) << "Update message";
        (*(it-1)) = message;
        emitChanged(std::distance(mAllMessages.begin(), it - 1));
    } else {
        const int pos = it - mAllMessages.begin();
        beginInsertRows(QModelIndex(), pos, pos);
        mAllMessages.insert(it, message);
        endInsertRows();
    }
}

void MessageModel::addMessages(const QVector<Message> &messages)
{
    if (mAllMessages.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, messages.count() - 1);
        mAllMessages = messages;
        std::sort(mAllMessages.begin(), mAllMessages.end(), compareTimeStamps);
        endInsertRows();
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
    case MessageModel::MessageConvertedText:
        if (message.messageType() == Message::System) {
            return message.systemMessageText();
        } else {
            if (mRoom && mRoom->userIsIgnored(message.userId())) {
                return QString(QStringLiteral("<i>") + i18n("Ignored Message") + QStringLiteral("</i>"));
            }
            const QString userName = mRocketChatAccount ? mRocketChatAccount->userName() : QString();
            return convertMessageText(message, userName);
        }

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
    case MessageModel::Attachments:
    {
        QVariantList lst;
        lst.reserve(message.attachements().count());
        const auto attachs = message.attachements();
        for (const MessageAttachment &att : attachs) {
            lst.append(QVariant::fromValue(att));
        }
        return lst;
    }
    case MessageModel::Urls:
    {
        QVariantList lst;
        lst.reserve(message.urls().count());
        const auto urls = message.urls();
        for (const MessageUrl &url : urls) {
            lst.append(QVariant::fromValue(url));
        }
        return lst;
    }
    case MessageModel::Date:
    {
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
        return (message.timeStamp() + (mRocketChatAccount ? mRocketChatAccount->ruqolaServerConfig()->blockEditingMessageInMinutes() * 60 * 1000 : 0))
               > QDateTime::currentMSecsSinceEpoch();
    case MessageModel::Starred:
        return message.starred();
    case MessageModel::UsernameUrl:
    {
        const QString username = message.username();
        if (username.isEmpty()) {
            return {};
        }
        return QStringLiteral("<a href=\'ruqola:/user/%1\'>@%1</a>").arg(message.username());
    }
    case MessageModel::Roles:
    {
        const QString str = roomRoles(message.userId()).join(QLatin1Char(','));
        return str;
    }
    case MessageModel::Reactions:
    {
        QVariantList lst;
        const auto reactions = message.reactions().reactions();
        lst.reserve(reactions.count());
        for (const Reaction &react : reactions) {
            //Convert reactions
            lst.append(QVariant::fromValue(react));
        }
        return lst;
    }
    case MessageModel::Ignored:
        return mRoom && mRoom->userIsIgnored(message.userId());
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
    {
        const QString userName = mRocketChatAccount ? mRocketChatAccount->userName() : QString();
        return threadMessagePreview(message.threadMessageId(), userName);
    }
    case MessageModel::Groupable:
        return message.groupable();
    case MessageModel::ShowTranslatedMessage:
        return message.showTranslatedMessage();
    case MessageModel::DisplayAttachment:
        return message.showAttachment();
    }

    return {};
}

bool MessageModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        qCWarning(RUQOLA_LOG) << "ERROR: invalid index";
        return {};
    }
    const int idx = index.row();
    Message &message = mAllMessages[idx];

    switch (role) {
    case MessageModel::DisplayAttachment:
        message.setShowAttachment(value.toBool());
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
    return QStringList();
}

QString MessageModel::convertMessageText(const Message &message, const QString &userName) const
{
    QString messageStr = message.text();
    if (message.showTranslatedMessage() && mRoom && mRoom->autoTranslate() && !mRoom->autoTranslateLanguage().isEmpty()) {
        const QString messageTranslation = message.messageTranslation().translatedStringFromLanguage(mRoom->autoTranslateLanguage());
        if (!messageTranslation.isEmpty()) {
            messageStr = messageTranslation;
        }
        //qDebug() << " autotranslate true && mRoom->autoTranslateLanguage() :" << mRoom->autoTranslateLanguage();
    }

    return mTextConverter->convertMessageText(messageStr, userName, mAllMessages);
}

void MessageModel::setRoomId(const QString &roomID)
{
    mRoomId = roomID;
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
    auto it = std::find_if(mAllMessages.begin(), mAllMessages.end(), [messageId](const Message &msg) {
        return msg.messageId() == messageId;
    });
    if (it != mAllMessages.end()) {
        //TODO
    }
}

void MessageModel::slotFileDownloaded(const QString &filePath, const QUrl &cacheImageUrl)
{
    Q_UNUSED(cacheImageUrl)
    auto matchesFilePath = [&](const QVector<MessageAttachment> &msgAttachments) {
                               return std::find_if(msgAttachments.begin(), msgAttachments.end(), [&](const MessageAttachment &attach) {
            return attach.link() == filePath;
        }) != msgAttachments.end();
                           };
    auto it = std::find_if(mAllMessages.begin(), mAllMessages.end(), [&](const Message &msg) {
        if (msg.messageType() == Message::Image) {
            return matchesFilePath(msg.attachements());
        }
        return false;
    });
    if (it != mAllMessages.end()) {
        const QModelIndex idx = createIndex(std::distance(mAllMessages.begin(), it), 0);
        Q_EMIT dataChanged(idx, idx);
    }
}

void MessageModel::changeDisplayAttachment(const QString &messageId, bool displayAttachment)
{
    auto it = std::find_if(mAllMessages.begin(), mAllMessages.end(), [messageId](const Message &msg) {
        return msg.messageId() == messageId;
    });
    if (it != mAllMessages.end()) {
        (*it).setShowAttachment(displayAttachment);
    }
}

void MessageModel::deleteMessage(const QString &messageId)
{
    auto it = std::find_if(mAllMessages.begin(), mAllMessages.end(), [messageId](const Message &msg) {
        return msg.messageId() == messageId;
    });
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

QString MessageModel::threadMessagePreview(const QString &threadMessageId, const QString &userName) const
{
    if (!threadMessageId.isEmpty()) {
        auto it = std::find_if(mAllMessages.cbegin(), mAllMessages.cend(), [threadMessageId](const Message &msg) {
            return msg.messageId() == threadMessageId;
        });
        if (it != mAllMessages.cend()) {
            QString str = convertMessageText((*it), userName);
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

QString MessageModel::roomId() const
{
    return mRoomId;
}
