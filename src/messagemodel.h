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

#include "libruqolacore_export.h"
#include <QAbstractListModel>
#include <QStringList>
#include <QObject>
#include <QByteArray>
#include <QJsonObject>
#include <QFile>

class Message //: public QObject
{
//       Q_OBJECT
public:

//    enum MessageStatus {
//        Unsent,
//        Sending,
//        Sent,
//        SendFailed
//    };
//    Q_ENUM(MessageStatus)

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

//    MessageStatus messageStatus() const;
//    void setMessageStatus(MessageStatus m);

    //Message Object Fields

    // _id
    QString messageID;

    // rid
    QString roomID;

    // msg
    QString message;

    // ts
    qint64 timestamp;

    // u
    QString username;
    QString userID;

    // _updatedAt
    qint64 updatedAt;

    // editedAt
    qint64 editedAt;

    // editedBy
    QString editedByUsername;
    QString editedByUserID;

    // urls
    QString url;
    QString meta;
    QString headers;
    QString parsedUrl;

    // attachments
    QString imageUrl;
    QString color;

    // alias
    QString alias;

    // avatar
    QString avatar;

    // groupable
    bool groupable;

    // parseUrls
    bool parseUrls;

    bool systemMessage = false;
    QString systemMessageType;

//    MessageStatus m_messageStatus;

signals:
//    void messageStatusChanged();
};

class LIBRUQOLACORE_EXPORT MessageModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum MessageRoles {
        Username = Qt::UserRole + 1,
        MessageText,
        Timestamp,
        UserID,
        SystemMessage,
        SystemMessageType,
        MessageID,
        RoomID,
        UpdatedAt,
        EditedAt,
        EditedByUserName,
        EditedByUserID,
        Url,
        Meta,
        Headers,
        ParsedUrl,
        ImageUrl,
        Color,
        Alias,
        Avatar,
        Groupable,
        ParseUrls
    };
    Q_ENUM(MessageRoles)

    explicit MessageModel(const QString &roomID = QStringLiteral("no_room"), QObject *parent = nullptr);
    virtual ~MessageModel();

    /**
    * @brief Adds a message to QVector m_allMessages
    *
    * @param message The message to be added
    */
    void addMessage(const Message &message);

    /**
    * @brief returns number of messages in QVector m_allMessages
    *
    * @param parent, it is void
    * @return int, The number of messages in QVector m_allMessages
    */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /**
    * @brief Returns last timestamp of last message in QVector m_allMessages
    *
    * @return qint64 The last timestamp
    */
    qint64 lastTimestamp() const;

    /**
    * @brief Constructs Message object from QJsonObject
    *
    * @param source The Json containing message attributes
    * @return Message object, The message constructed from Json
    */
    static Message fromJSon(const QJsonObject &source);

    /**
    * @brief Constructs QBytearray from Message object
    *
    * @param message The Message object
    * @return QByteArray, The Json containing message attributes
    */
    static QByteArray serialize(const Message &message);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    const QString m_roomID;

    QVector<Message> m_allMessages;

    QString m_writableLocation;
};

#endif
