
#include <QtCore>
#include <QModelIndex>
#include <QAbstractListModel>

#include "messagemodel.h"
/*
Message::Message(const QString& username, const QString& message, qulonglong timestamp)
 : m_username(username), m_message(message), m_timestamp(timestamp)
{
}

QString Message::message() const
{
    return m_message;
}

QString Message::username() const
{
    return m_username;
}

qulonglong Message::timestamp() const
{
    return m_timestamp;
}*/

MessageModel::MessageModel(QObject* parent)
  : QAbstractListModel(parent),
  m_currentRoom("no_room")
{
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
/*
void MessageModel::addMessage(qulonglong timestamp, const QString& user, const QString& message)
{
//     qDebug() << roomID << user << message;
    Message m;
    m.username = user;
    m.message = message;
    m.timestamp = timestamp;
    addMessage(m);
}*/

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
    
//     qDebug() << "PROTECTED ADD MESSAGE" << roomID << m_currentRoom;
    int size = m_allMessages.size();
        
//         qDebug() << "calling begin insert rows:" << size << size+1;
    
    bool messageChanged = false;
    if (m_allMessages.contains(message.timestamp)) {
        messageChanged = true;
        //Figure out a better way to update just the really changed message
    } else {
        beginInsertRows(index(size),  size, (size));
    }
    
    m_allMessages[message.timestamp] = message;
    
    if (messageChanged) {
        qDebug() << "Data changed";
        //Figure out a better way to update just the really changed message, not EVERYTHING
        emit dataChanged(createIndex(1, 1), createIndex(rowCount(), 1), QVector<int>(MessageModel::MessageText));
        
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
        return  m_allMessages.values().at(idx).username;
    } else if (role == MessageModel::MessageText) {
        return  m_allMessages.values().at(idx).message;
    }  else if (role == MessageModel::Timestamp) {
        return  QVariant(m_allMessages.values().at(idx).timestamp);
    } else if (role == MessageModel::UserID) {
        return  QVariant(m_allMessages.values().at(idx).userID);
    } else if (role == MessageModel::SystemMessage) {
//         qDebug() << "System message?" << m_allMessages.values().at(idx).systemMessage;
        return  QVariant(m_allMessages.values().at(idx).systemMessage);
    } else if (role == MessageModel::SystemMessageType) {
        return  QVariant(m_allMessages.values().at(idx).systemMessageType);
    } else {
        return QVariant("");
    }
}

// #include "messagelist.moc"
