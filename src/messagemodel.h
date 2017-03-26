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
