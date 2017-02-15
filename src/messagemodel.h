
#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QObject>
#include <QByteArray>
#include <QJsonObject>
#include <QFile>

class Message {
public:
    // To be used in ID find: message ID
    inline bool operator==(const Message &other) const
    {
        return other.messageID == messageID;
    }
    // To be used in sorted insert: timestamp
    inline bool operator<(const Message &other) const
    {
        return timestamp < other.timestamp;
    }
    
    QString messageID;
    
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

    MessageModel(const QString &roomID = "no_room", QObject *parent = 0);
    virtual ~MessageModel();

    void addMessage(const Message& message);

    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    qint64 lastTimestamp() const;
    
    static Message fromJSon(const QJsonObject &source);
    static QByteArray serialize(const Message &message);
protected:

    virtual QHash<int, QByteArray> roleNames() const;
private:
    const QString m_roomID;
    
    QVector<Message> m_allMessages;
    
//     QMap<int, Message> m_allMessages;
//     QMap<int, Message> m_allMessages;
    QString m_writableLocation;
    
    QFile *cacheWriter;
};

#endif
