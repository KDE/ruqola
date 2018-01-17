/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef MESSAGE_H
#define MESSAGE_H

#include "libruqola_private_export.h"
#include "messageattachment.h"
#include "messageurl.h"
#include <QJsonObject>
#include <QString>
#include <QVector>
#include <QObject>

class LIBRUQOLACORE_TESTS_EXPORT Message
{
    Q_GADGET
public:
    Message();

    enum MessageType {
        System,
        NormalText,
        File,
        Video,
        Audio,
        Image
    };

    Q_ENUM(MessageType)

    QString roomId() const;
    void setRoomId(const QString &roomId);

    bool groupable() const;
    void setGroupable(bool groupable);

    bool parseUrls() const;
    void setParseUrls(bool parseUrls);

    QString avatar() const;
    void setAvatar(const QString &avatar);

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

    void parseMessage(const QJsonObject &o);

    bool operator==(const Message &other) const;

    Message &operator=(const Message &other);

    // To be used in sorted insert: timestamp
    bool operator<(const Message &other) const;


    QString messageId() const;
    void setMessageId(const QString &messageId);

    QString text() const;
    void setText(const QString &text);

    qint64 timeStamp() const;
    void setTimeStamp(const qint64 &timeStamp);

    QString username() const;
    void setUsername(const QString &username);

    QString userId() const;
    void setUserId(const QString &userId);

    qint64 updatedAt() const;
    void setUpdatedAt(const qint64 &updatedAt);

    qint64 editedAt() const;
    void setEditedAt(const qint64 &editedAt);

    QString editedByUsername() const;
    void setEditedByUsername(const QString &editedByUsername);

    QString editedByUserId() const;
    void setEditedByUserId(const QString &editedByUserId);

    QString imageUrl() const;
    void setImageUrl(const QString &imageUrl);

    QString alias() const;
    void setAlias(const QString &alias);

    QString systemMessageType() const;
    void setSystemMessageType(const QString &systemMessageType);

    MessageType messageType() const;
    void setMessageType(const MessageType &messageType);

    QVector<MessageAttachment> attachements() const;
    void setAttachements(const QVector<MessageAttachment> &attachements);

    QVector<MessageUrl> urls() const;
    void setUrls(const QVector<MessageUrl> &urls);

    QMap<QString, QString> mentions() const;
    void setMentions(const QMap<QString, QString> &mentions);

private:
    void parseMentions(const QJsonArray &mentions);
    void parseAttachment(const QJsonArray &attachments);
    void parseUrls(const QJsonArray &urls);
    //Message Object Fields
    QVector<MessageAttachment> mAttachements;

    //Message urls object
    QVector<MessageUrl> mUrls;

    //Mentions
    QMap<QString, QString> mMentions;

    // _id
    QString mMessageId;

    // msg
    QString mText;

    // u
    QString mUsername;
    QString mUserId;

    // editedBy
    QString mEditedByUsername;
    QString mEditedByUserId;

    // alias
    QString mAlias;

    QString mSystemMessageType;

    // rid
    QString mRoomId;

    // avatar
    QString mAvatar;

    // ts
    qint64 mTimeStamp = -1;
    // _updatedAt
    qint64 mUpdatedAt = -1;

    // editedAt
    qint64 mEditedAt = -1;

    MessageType mMessageType = MessageType::NormalText;
    // groupable
    bool mGroupable = false;

    // parseUrls
    bool mParseUrls = false;
};
Q_DECLARE_METATYPE(Message)
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const Message &t);
#endif // MESSAGE_H
