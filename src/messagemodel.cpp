/*
 * <one line to give the program's name and a brief idea of what it does.>
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
#include "ruqola.h"
#include "ruqola_debug.h"


//Message::MessageStatus Message::messageStatus() const
//{
//    return m_messageStatus;
//}

//void Message::setMessageStatus(MessageStatus m)
//{
//    if (m_messageStatus!= m){
//        m_messageStatus = m;
//        emit messageStatusChanged();
//    }
//}


Message MessageModel::fromJSon(const QJsonObject& o)
{
    Message message;

    message.messageID = o[QStringLiteral("messageID")].toString();
    message.roomID = o[QStringLiteral("roomID")].toString();
    message.message = o[QStringLiteral("message")].toString();
    message.timestamp = (qint64) o[QStringLiteral("timestamp")].toDouble();
    message.username = o[QStringLiteral("username")].toString();
    message.userID = o[QStringLiteral("userID")].toString();
    message.updatedAt = (qint64) o[QStringLiteral("updatedAt")].toDouble();
    message.editedAt = (qint64) o[QStringLiteral("editedAt")].toDouble();
    message.editedByUsername = o[QStringLiteral("editedByUsername")].toString();
    message.editedByUserID = o[QStringLiteral("editedByUserID")].toString();
    message.url = o[QStringLiteral("url")].toString();
    message.meta = o[QStringLiteral("meta")].toString();
    message.headers = o[QStringLiteral("headers")].toString();
    message.parsedUrl = o[QStringLiteral("parsedUrl")].toString();
    message.imageUrl = o[QStringLiteral("imageUrl")].toString();
    message.color = o[QStringLiteral("color")].toString();
    message.alias = o[QStringLiteral("alias")].toString();
    message.avatar = o[QStringLiteral("avatar")].toString();
    message.groupable = o[QStringLiteral("groupable")].toBool();
    message.parseUrls = o[QStringLiteral("parseUrls")].toBool();

    message.systemMessage = o[QStringLiteral("systemMessage")].toBool();
    message.systemMessageType = o[QStringLiteral("type")].toString();

    return message;
}

QByteArray MessageModel::serialize(const Message& message)
{
    QJsonDocument d;
    QJsonObject o;

    o[QStringLiteral("messageID")] = message.messageID;
    o[QStringLiteral("roomID")] = message.roomID;
    o[QStringLiteral("message")] = message.message;
    o[QStringLiteral("timestamp")] = message.timestamp;
    o[QStringLiteral("username")] = message.username;
    o[QStringLiteral("userID")] = message.userID;
    o[QStringLiteral("updatedAt")] = message.updatedAt;
    o[QStringLiteral("editedAt")] = message.editedAt;
    o[QStringLiteral("editedByUsername")] = message.editedByUsername;
    o[QStringLiteral("editedByUserID")] = message.editedByUserID;
    o[QStringLiteral("url")] = message.url;
    o[QStringLiteral("meta")] = message.meta;
    o[QStringLiteral("headers")] = message.headers;
    o[QStringLiteral("parsedUrl")] = message.parsedUrl;
    o[QStringLiteral("imageUrl")] = message.imageUrl;
    o[QStringLiteral("color")] = message.color;
    o[QStringLiteral("alias")] = message.alias;
    o[QStringLiteral("avatar")] = message.avatar;
    o[QStringLiteral("groupable")] = message.groupable;
    o[QStringLiteral("parseUrls")] = message.parseUrls;

    o[QStringLiteral("systemMessage")] = message.systemMessage;
    o[QStringLiteral("type")] = message.systemMessageType;

    d.setObject(o);
    return d.toBinaryData();
}

MessageModel::MessageModel(const QString &roomID, QObject* parent)
  : QAbstractListModel(parent),
  m_roomID(roomID)
{
    qCDebug(RUQOLA_LOG) << "Creating message Model";
    QDir cacheDir(Ruqola::self()->cacheBasePath()+QStringLiteral("/rooms_cache"));
    
        // load cache
    if (QFile::exists(cacheDir.absoluteFilePath(roomID)) && !roomID.isEmpty()) {
        QFile f(cacheDir.absoluteFilePath(roomID));
        if (f.open(QIODevice::ReadOnly)) {
            QDataStream in(&f);
            while (!f.atEnd()) {
                char * byteArray;
                quint32 length;
                in.readBytes(byteArray, length);
                QByteArray arr = QByteArray::fromRawData(byteArray, length);
                Message m = MessageModel::fromJSon(QJsonDocument::fromBinaryData(arr).object());
                addMessage(m);
            }
        }
    }   
}

MessageModel::~MessageModel()
{
    QDir cacheDir(Ruqola::self()->cacheBasePath()+QStringLiteral("/rooms_cache"));
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
        qCDebug(RUQOLA_LOG) << "returning timestamp" << m_allMessages.last().timestamp;
        return m_allMessages.last().timestamp;
    } else {
        return 0;
    }
}


int MessageModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_allMessages.size();
}

void MessageModel::addMessage(const Message& message)
{
    // Don't add empty messages
    if (message.message.isEmpty()) {
        return;
    }

    auto existingMessage = std::find(m_allMessages.begin(), m_allMessages.end(), message);
    bool present = (existingMessage != m_allMessages.end());
    auto i = std::upper_bound(m_allMessages.begin(), m_allMessages.end(), message);
    int pos = i-m_allMessages.begin();
    bool messageChanged = false;
    
//     if (qFind(m_allMessages.begin(), m_allMessages.end(), message) != m_allMessages.end()) {
    if (present){
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
        emit dataChanged(createIndex(1, 1), createIndex(pos, 1));
        
    } else {
        endInsertRows();
    }
}

QVariant MessageModel::data(const QModelIndex& index, int role) const
{
    
    int idx = index.row();
    if (role == MessageModel::Username) {
        return  m_allMessages.at(idx).username;
    } else if (role == MessageModel::MessageText) {
        return  m_allMessages.at(idx).message;
    }  else if (role == MessageModel::Timestamp) {
        return  QVariant(m_allMessages.at(idx).timestamp);
    } else if (role == MessageModel::UserID) {
        return  QVariant(m_allMessages.at(idx).userID);
    } else if (role == MessageModel::SystemMessage) {
        return  QVariant(m_allMessages.at(idx).systemMessage);
    } else if (role == MessageModel::SystemMessageType) {
        return  QVariant(m_allMessages.at(idx).systemMessageType);
    } else if (role == MessageModel::MessageID) {
        return  QVariant(m_allMessages.at(idx).messageID);
    }else if (role == MessageModel::Alias) {
        return  QVariant(m_allMessages.at(idx).alias);
    } else {
        return QVariant(QString());
    }
}

// #include "messagelist.moc"
