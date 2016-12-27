
#include <QFile>
#include <QVector>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDataStream>
// #include <QtCore>
#include <QModelIndex>
#include <QAbstractListModel>

#include "messagemodel.h"
#include "userdata.h"

Message MessageModel::fromJSon(const QJsonObject& o)
{
    Message message;
    message.username = o["username"].toString();
    message.message = o["message"].toString();
    message.userID = o["userID"].toString();
    message.timestamp = (qint64) o["timestamp"].toDouble();
    message.systemMessage = o["systemMessage"].toBool();
    message.systemMessageType = o["type"].toString();
    message.roomID = o["roomID"].toString();
    message.messageID = o["messageID"].toString();
    
    return message;
}

QByteArray MessageModel::serialize(const Message& message)
{
    QJsonDocument d;
    QJsonObject o;
    o["username"] = message.username;
    o["message"] = message.message;
    o["userID"] = message.userID;
    o["timestamp"] = message.timestamp;
    o["systemMessage"] = message.systemMessage;
    o["type"] = message.systemMessageType;
    o["roomID"] = message.roomID;
    o["messageID"] = message.messageID;
    d.setObject(o);
    return d.toBinaryData();
}

MessageModel::MessageModel(const QString &roomID, QObject* parent)
  : QAbstractListModel(parent),
  m_roomID(roomID)
{
    qDebug() << "Creating message Model";
    QDir cacheDir(UserData::self()->cacheBasePath()+"/rooms_cache");
    
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
//                 m_allMessages[m.timestamp] = m;
//                 qDebug() << m.message;
            }
        }
    }   
}

MessageModel::~MessageModel()
{
    QDir cacheDir(UserData::self()->cacheBasePath()+"/rooms_cache");
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
//     qDebug() << "C++ asked for rowcount " <<  m_allMessages.size();
//     if (m_allMessages.contains(m_currentRoom)) {
    return m_allMessages.size();

    (void)parent;
}

void MessageModel::addMessage(const Message& message)
{
    // Don't add empty messages?
    if (message.message.isEmpty()) {
        return;
    }
    
    if (UserData::self()->loginStatus() != DDPClient::LoggedIn) {
        return;
    }
    
    QVector<Message>::iterator i = qUpperBound(m_allMessages.begin(), m_allMessages.end(),
                                               message);
    
    int pos = i-m_allMessages.begin();
    
    bool messageChanged = false;
    
//     if (qFind(m_allMessages.begin(), m_allMessages.end(), message) != m_allMessages.end()) {
    if (false) {
        messageChanged = true;
        //Figure out a better way to update just the really changed message
    } else {
        beginInsertRows(QModelIndex(), pos, pos);
    }
    
    m_allMessages.insert(i, message);
    
    if (messageChanged) {
        emit dataChanged(createIndex(1, 1), createIndex(pos, 1));
        
    } else {
        endInsertRows();
    }
}

QVariant MessageModel::data(const QModelIndex& index, int role) const
{
//     return QVariant("Hey baby");
//     qDebug() << "C++ got asked item at index" << index
//              << "room contains" << m_allMessages[m_currentRoom].size() << "messages";
//     foreach (Message m, m_allMessages[m_currentRoom].values()) {
//         qDebug() << m.username();
//     }
    
    int idx = index.row();//-1;
    if (role == MessageModel::Username) {
//         qDebug() << "C++ returning username" <<
//                     m_allMessages[m_currentRoom].values().at(idx).username();
        return  m_allMessages.at(idx).username;
    } else if (role == MessageModel::MessageText) {
        return  m_allMessages.at(idx).message;
    }  else if (role == MessageModel::Timestamp) {
        return  QVariant(m_allMessages.at(idx).timestamp);
    } else if (role == MessageModel::UserID) {
        return  QVariant(m_allMessages.at(idx).userID);
    } else if (role == MessageModel::SystemMessage) {
//         qDebug() << "System message?" << m_allMessages.at(idx).systemMessage;
        return  QVariant(m_allMessages.at(idx).systemMessage);
    } else if (role == MessageModel::SystemMessageType) {
        return  QVariant(m_allMessages.at(idx).systemMessageType);
    } else {
        return QVariant("");
    }
}

// #include "messagelist.moc"
