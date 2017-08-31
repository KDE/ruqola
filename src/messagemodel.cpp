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
#include <QVector>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDataStream>
#include <QModelIndex>
#include <QAbstractListModel>

#include "messagemodel.h"
#include "message.h"
#include "ruqola_debug.h"
#include "rocketchataccount.h"

MessageModel::MessageModel(const QString &roomID, RocketChatAccount *account, QObject *parent)
    : QAbstractListModel(parent)
    , m_roomID(roomID)
    , mRocketChatAccount(account)
{
    qCDebug(RUQOLA_LOG) << "Creating message Model";
    QDir cacheDir(mRocketChatAccount->settings()->cacheBasePath()+QStringLiteral("/rooms_cache"));

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
                Message m = MessageModel::fromJSon(QJsonDocument::fromBinaryData(arr).object());
                addMessage(m);
            }
        }
    }
}

MessageModel::~MessageModel()
{
    QDir cacheDir(mRocketChatAccount->settings()->cacheBasePath()+QStringLiteral("/rooms_cache"));
    qCDebug(RUQOLA_LOG) << "Caching to..." << cacheDir.path();
    if (!cacheDir.exists(cacheDir.path())) {
        cacheDir.mkpath(cacheDir.path());
    }

    QFile f(cacheDir.absoluteFilePath(m_roomID));

    if (f.open(QIODevice::WriteOnly)) {
        QDataStream out(&f);
        for (const Message &m : qAsConst(m_allMessages)) {
            const QByteArray ms = MessageModel::serialize(m);
            out.writeBytes(ms, ms.size());
        }
    }
}

Message MessageModel::fromJSon(const QJsonObject &o)
{
    Message message;

    message.mMessageId = o[QStringLiteral("messageID")].toString();
    message.mRoomId = o[QStringLiteral("roomID")].toString();
    message.mText = o[QStringLiteral("message")].toString();
    message.mTimeStamp = (qint64)o[QStringLiteral("timestamp")].toDouble();
    message.mUsername = o[QStringLiteral("username")].toString();
    message.mUserId = o[QStringLiteral("userID")].toString();
    message.mUpdatedAt = (qint64)o[QStringLiteral("updatedAt")].toDouble();
    message.mEditedAt = (qint64)o[QStringLiteral("editedAt")].toDouble();
    message.mEditedByUsername = o[QStringLiteral("editedByUsername")].toString();
    message.mEditedByUserId = o[QStringLiteral("editedByUserID")].toString();
    message.mUrl = o[QStringLiteral("url")].toString();
    message.mMeta = o[QStringLiteral("meta")].toString();
    message.mHeaders = o[QStringLiteral("headers")].toString();
    message.mParsedUrl = o[QStringLiteral("parsedUrl")].toString();
    message.mImageUrl = o[QStringLiteral("imageUrl")].toString();
    message.mColor = o[QStringLiteral("color")].toString();
    message.mAlias = o[QStringLiteral("alias")].toString();
    message.mAvatar = o[QStringLiteral("avatar")].toString();
    message.mGroupable = o[QStringLiteral("groupable")].toBool();
    message.mParseUrls = o[QStringLiteral("parseUrls")].toBool();

    message.mSystemMessage = o[QStringLiteral("systemMessage")].toBool();
    message.mSystemMessageType = o[QStringLiteral("type")].toString();

    return message;
}

QByteArray MessageModel::serialize(const Message &message)
{
    QJsonDocument d;
    QJsonObject o;

    o[QStringLiteral("messageID")] = message.mMessageId;
    o[QStringLiteral("roomID")] = message.mRoomId;
    o[QStringLiteral("message")] = message.mText;
    o[QStringLiteral("timestamp")] = message.mTimeStamp;
    o[QStringLiteral("username")] = message.mUsername;
    o[QStringLiteral("userID")] = message.mUserId;
    o[QStringLiteral("updatedAt")] = message.mUpdatedAt;
    o[QStringLiteral("editedAt")] = message.mEditedAt;
    o[QStringLiteral("editedByUsername")] = message.mEditedByUsername;
    o[QStringLiteral("editedByUserID")] = message.mEditedByUserId;
    o[QStringLiteral("url")] = message.mUrl;
    o[QStringLiteral("meta")] = message.mMeta;
    o[QStringLiteral("headers")] = message.mHeaders;
    o[QStringLiteral("parsedUrl")] = message.mParsedUrl;
    o[QStringLiteral("imageUrl")] = message.mImageUrl;
    o[QStringLiteral("color")] = message.mColor;
    o[QStringLiteral("alias")] = message.mAlias;
    o[QStringLiteral("avatar")] = message.mAvatar;
    o[QStringLiteral("groupable")] = message.mGroupable;
    o[QStringLiteral("parseUrls")] = message.mParseUrls;

    o[QStringLiteral("systemMessage")] = message.mSystemMessage;
    o[QStringLiteral("type")] = message.mSystemMessageType;

    d.setObject(o);
    return d.toBinaryData();
}

QHash<int, QByteArray> MessageModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[MessageText] = "messageText";
    roles[Username] = "username";
    roles[Timestamp] = "timestamp";
    roles[UserID] = "userID";
    roles[SystemMessage] = "systemMessage";
    roles[SystemMessageType] = "type";
    roles[MessageID] = "messageID";
    roles[RoomID] = "roomID";
    roles[UpdatedAt] = "updatedAt";
    roles[EditedAt] = "editedAt";
    roles[EditedByUserName] = "editedByUsername";
    roles[EditedByUserID] = "editedByUserID";
    roles[Url] = "URL";
    roles[Meta] = "meta";
    roles[Headers] = "headers";
    roles[ParsedUrl] = "parsedUrl";
    roles[ImageUrl] = "imageUrl";
    roles[Color] = "color";
    roles[Alias] = "alias";
    roles[Avatar] = "avatar";
    roles[Groupable] = "groupable";
    roles[ParseUrls] = "parseUrls";

    return roles;
}

qint64 MessageModel::lastTimestamp() const
{
    if (m_allMessages.size()) {
        qCDebug(RUQOLA_LOG) << "returning timestamp" << m_allMessages.last().mTimeStamp;
        return m_allMessages.last().mTimeStamp;
    } else {
        return 0;
    }
}

int MessageModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_allMessages.size();
}

void MessageModel::addMessage(const Message &message)
{
    // Don't add empty messages
    if (message.mText.isEmpty()) {
        return;
    }

    auto existingMessage = std::find(m_allMessages.begin(), m_allMessages.end(), message);
    bool present = (existingMessage != m_allMessages.end());
    auto i = std::upper_bound(m_allMessages.begin(), m_allMessages.end(), message);
    int pos = i-m_allMessages.begin();
    bool messageChanged = false;

//     if (qFind(m_allMessages.begin(), m_allMessages.end(), message) != m_allMessages.end()) {
    if (present) {
//     if (pos != m_allMessages.size()) { // we're at the end
//         qCDebug(RUQOLA_LOG) << "detecting a message change";
        messageChanged = true;
        //Figure out a better way to update just the really changed message
    } else {
        beginInsertRows(QModelIndex(), pos, pos);
    }

    if (messageChanged) {
        m_allMessages.replace(pos-1, message);
    } else {
        m_allMessages.insert(i, message);
    }

    if (messageChanged) {
        Q_EMIT dataChanged(createIndex(1, 1), createIndex(pos, 1));
    } else {
        endInsertRows();
    }
}

QVariant MessageModel::data(const QModelIndex &index, int role) const
{
    int idx = index.row();
    switch (role) {
    case MessageModel::Username:
        return m_allMessages.at(idx).mUsername;
    case MessageModel::MessageText:
        return m_allMessages.at(idx).mText;
    case MessageModel::Timestamp:
        return m_allMessages.at(idx).mTimeStamp;
    case MessageModel::UserID:
        return m_allMessages.at(idx).mUserId;
    case MessageModel::SystemMessage:
        return m_allMessages.at(idx).mSystemMessage;
    case MessageModel::SystemMessageType:
        return m_allMessages.at(idx).mSystemMessageType;
    case MessageModel::MessageID:
        return m_allMessages.at(idx).mMessageId;
    case MessageModel::Alias:
        return m_allMessages.at(idx).mAlias;
    default:
        return QString();
    }
}

bool Message::isEqual(const Message &other) const
{
    return (mMessageId == other.mMessageId)
           && (mRoomId == other.mRoomId)
           && (mText == other.mText)
           && (mTimeStamp == other.mTimeStamp)
           && (mUsername == other.mUsername)
           && (mUserId == other.mUserId)
           && (mUpdatedAt == other.mUpdatedAt)
           && (mEditedAt == other.mEditedAt)
           && (mEditedByUsername == other.mEditedByUsername)
           && (mEditedByUserId == other.mEditedByUserId)
           && (mUrl == other.mUrl)
           && (mMeta == other.mMeta)
           && (mHeaders == other.mHeaders)
           && (mParsedUrl == other.mParsedUrl)
           && (mImageUrl == other.mImageUrl)
           && (mColor == other.mColor)
           && (mAlias == other.mAlias)
           && (mAvatar == other.mAvatar)
           && (mSystemMessageType == other.mSystemMessageType)
           && (mGroupable == other.mGroupable)
           && (mParseUrls == other.mParseUrls)
           && (mSystemMessage == other.mSystemMessage);
}
