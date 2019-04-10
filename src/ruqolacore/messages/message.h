/*
   Copyright (c) 2017-2019 Montel Laurent <montel@kde.org>

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
#include "messagepinned.h"
#include "messagestarred.h"
#include "reactions.h"
#include <QJsonObject>
#include <QString>
#include <QVector>

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

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

    Q_REQUIRED_RESULT bool groupable() const;
    void setGroupable(bool groupable);

    Q_REQUIRED_RESULT bool parseUrls() const;
    void setParseUrls(bool parseUrls);

    Q_REQUIRED_RESULT QString avatar() const;
    void setAvatar(const QString &avatar);

    /**
    * @brief Constructs Message object from QJsonObject
    *
    * @param source The Json containing message attributes
    * @return Message object, The message constructed from Json
    */
    Q_REQUIRED_RESULT static Message fromJSon(const QJsonObject &source);

    /**
    * @brief Constructs QBytearray from Message object
    *
    * @param message The Message object
    * @return QByteArray, The Json containing message attributes
    */
    Q_REQUIRED_RESULT static QByteArray serialize(const Message &message, bool toBinary = true);

    void parseMessage(const QJsonObject &o, bool restApi = false);

    Q_REQUIRED_RESULT bool operator==(const Message &other) const;

    Message &operator=(const Message &other);

    // To be used in sorted insert: timestamp
    bool operator<(const Message &other) const;

    Q_REQUIRED_RESULT QString messageId() const;
    void setMessageId(const QString &messageId);

    Q_REQUIRED_RESULT QString text() const;
    void setText(const QString &text);

    Q_REQUIRED_RESULT qint64 timeStamp() const;
    void setTimeStamp(const qint64 &timeStamp);

    Q_REQUIRED_RESULT QString username() const;
    void setUsername(const QString &username);

    Q_REQUIRED_RESULT QString userId() const;
    void setUserId(const QString &userId);

    Q_REQUIRED_RESULT qint64 updatedAt() const;
    void setUpdatedAt(const qint64 &updatedAt);

    Q_REQUIRED_RESULT qint64 editedAt() const;
    void setEditedAt(const qint64 &editedAt);

    Q_REQUIRED_RESULT QString editedByUsername() const;
    void setEditedByUsername(const QString &editedByUsername);

    Q_REQUIRED_RESULT QString editedByUserId() const;
    void setEditedByUserId(const QString &editedByUserId);

    Q_REQUIRED_RESULT QString imageUrl() const;
    void setImageUrl(const QString &imageUrl);

    Q_REQUIRED_RESULT QString alias() const;
    void setAlias(const QString &alias);

    Q_REQUIRED_RESULT QString systemMessageType() const;
    void setSystemMessageType(const QString &systemMessageType);

    Q_REQUIRED_RESULT MessageType messageType() const;
    void setMessageType(const MessageType &messageType);

    Q_REQUIRED_RESULT QVector<MessageAttachment> attachements() const;
    void setAttachements(const QVector<MessageAttachment> &attachements);

    Q_REQUIRED_RESULT QVector<MessageUrl> urls() const;
    void setUrls(const QVector<MessageUrl> &urls);

    Q_REQUIRED_RESULT QMap<QString, QString> mentions() const;
    void setMentions(const QMap<QString, QString> &mentions);

    Q_REQUIRED_RESULT bool starred() const;
    void setStarred(bool starred);

    Q_REQUIRED_RESULT Reactions reactions() const;
    void setReactions(const Reactions &reactions);

    Q_REQUIRED_RESULT QString messageTypeText() const;

    Q_REQUIRED_RESULT QString role() const;
    void setRole(const QString &role);

    Q_REQUIRED_RESULT bool unread() const;
    void setUnread(bool unread);

    Q_REQUIRED_RESULT MessagePinned messagePinned() const;
    void setMessagePinned(const MessagePinned &messagePinned);

    Q_REQUIRED_RESULT MessageStarred messageStarred() const;
    void setMessageStarred(const MessageStarred &messageStarred);

private:
    void parseMentions(const QJsonArray &mentions);
    void parseAttachment(const QJsonArray &attachments);
    void parseUrls(const QJsonArray &urls);
    void parseReactions(const QJsonObject &mentions);

    //Message Pinned
    MessagePinned mMessagePinned;

    //Message Starred
    MessageStarred mMessageStarred;

    //Message Object Fields
    QVector<MessageAttachment> mAttachements;

    //Message urls object
    QVector<MessageUrl> mUrls;

    //Reactions
    Reactions mReactions;

    //Mentions
    QMap<QString, QString> mMentions;

    //role used when we add/remove role. It will displaying in messagesystem
    QString mRole;

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

    //Unread Message
    bool mUnread = false;
};
Q_DECLARE_METATYPE(Message)
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const Message &t);
#endif // MESSAGE_H
