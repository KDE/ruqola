
#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QObject>

// class Message
// {
// public:
//     Message(const QString &username = QString(),
//             const QString &message = QString(),
//             qulonglong timestamp = 0);
// 
//     QString username() const;
//     QString message() const;
//     qulonglong timestamp() const;
//     
// private:
//     QString m_username;
//     QString m_message;
//     qulonglong m_timestamp;
// };

struct Message {
    QString username;
    QString userID;
    QString message;
    qint64 timestamp;
    bool systemMessage = false;
    QString roomID;
    QString systemMessageType;
};
        
class MessageModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum MessageRoles {
        Username = Qt::UserRole + 1,
        MessageText,
        Timestamp,
        UserID,
        SystemMessage,
        SystemMessageType
    };

    MessageModel(QObject *parent = 0);

    void addMessage(const Message& message);

    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

//     static Message fromJSon(const QJSonDocument &source);
//     static QByteArray serialize(const Message &message);
protected:

    virtual QHash<int, QByteArray> roleNames() const;
private:
    QString m_currentRoom;
    
    // <room, <timestamp, Message> >
    QMap<int, Message> m_allMessages;
//     QMap<int, Message> m_roomMessages;
};

#endif
