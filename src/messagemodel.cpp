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
// #include <QtCore>
#include <QModelIndex>
#include <QAbstractListModel>

#include "messagemodel.h"
#include "ruqola.h"


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

    message.messageID = o["messageID"].toString();
    message.roomID = o["roomID"].toString();
    message.message = o["message"].toString();
    message.timestamp = (qint64) o["timestamp"].toDouble();
    message.username = o["username"].toString();
    message.userID = o["userID"].toString();
    message.updatedAt = (qint64) o["updatedAt"].toDouble();
    message.editedAt = (qint64) o["editedAt"].toDouble();
    message.editedByUsername = o["editedByUsername"].toString();
    message.editedByUserID = o["editedByUserID"].toString();
    message.url = o["url"].toString();
    message.meta = o["meta"].toString();
    message.headers = o["headers"].toString();
    message.parsedUrl = o["parsedUrl"].toString();
    message.imageUrl = o["imageUrl"].toString();
    message.color = o["color"].toString();
    message.alias = o["alias"].toString();
    message.avatar = o["avatar"].toString();
    message.groupable = o["groupable"].toBool();
    message.parseUrls = o["parseUrls"].toBool();

    message.systemMessage = o["systemMessage"].toBool();
    message.systemMessageType = o["type"].toString();

    
    return message;
}

QByteArray MessageModel::serialize(const Message& message)
{
    QJsonDocument d;
    QJsonObject o;

    o["messageID"] = message.messageID;
    o["roomID"] = message.roomID;
    o["message"] = message.message;
    o["timestamp"] = message.timestamp;
    o["username"] = message.username;
    o["userID"] = message.userID;
    o["updatedAt"] = message.updatedAt;
    o["editedAt"] = message.editedAt;
    o["editedByUsername"] = message.editedByUsername;
    o["editedByUserID"] = message.editedByUserID;
    o["url"] = message.url;
    o["meta"] = message.meta;
    o["headers"] = message.headers;
    o["parsedUrl"] = message.parsedUrl;
    o["imageUrl"] = message.imageUrl;
    o["color"] = message.color;
    o["alias"] = message.alias;
    o["avatar"] = message.avatar;
    o["groupable"] = message.groupable;
    o["parseUrls"] = message.parseUrls;

    o["systemMessage"] = message.systemMessage;
    o["type"] = message.systemMessageType;

    d.setObject(o);
    return d.toBinaryData();
}

MessageModel::MessageModel(const QString &roomID, QObject* parent)
  : QAbstractListModel(parent),
  m_roomID(roomID)
{
    qDebug() << "Creating message Model";
    QDir cacheDir(Ruqola::self()->cacheBasePath()+"/rooms_cache");
    
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
    QDir cacheDir(Ruqola::self()->cacheBasePath()+"/rooms_cache");
    qDebug() << "Caching to..." << cacheDir.path();
    if (!cacheDir.exists(cacheDir.path())) {
        cacheDir.mkpath(cacheDir.path());
    }
    
    QFile f(cacheDir.absoluteFilePath(m_roomID));

    if (f.open(QIODevice::WriteOnly)) {
        QDataStream out(&f);
        foreach (const Message m, m_allMessages) {            
            QByteArray ms = MessageModel::serialize(m);
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
        qDebug() << "returning timestamp" << m_allMessages.last().timestamp;
        return m_allMessages.last().timestamp;
    } else {
        return 0;
    }
}


int MessageModel::rowCount(const QModelIndex& parent) const
{
    return m_allMessages.size();
    (void)parent;
}

void MessageModel::addMessage(const Message& message)
{
    // Don't add empty messages
    if (message.message.isEmpty()) {
        return;
    }

    auto existingMessage = qFind(m_allMessages.begin(), m_allMessages.end(), message);
    bool present = (existingMessage != m_allMessages.end());
    auto i = std::upper_bound(m_allMessages.begin(), m_allMessages.end(), message);
    int pos = i-m_allMessages.begin();
    bool messageChanged = false;
    
//     if (qFind(m_allMessages.begin(), m_allMessages.end(), message) != m_allMessages.end()) {
    if (present){
//     if (pos != m_allMessages.size()) { // we're at the end
//         qDebug() << "detecting a message change";
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
    } else if (role == MessageModel::Alias) {
        return  QVariant(m_allMessages.at(idx).alias);
    } else {
        return QVariant("");
    }
}

// #include "messagelist.moc"
