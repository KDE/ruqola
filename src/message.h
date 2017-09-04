/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

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
#include <QJsonObject>
#include <QString>
#include <QVector>

class LIBRUQOLACORE_TESTS_EXPORT Message
{
public:
    Message();

    QString roomId() const;
    void setRoomId(const QString &roomId);

    bool groupable() const;
    void setGroupable(bool groupable);

    bool parseUrls() const;
    void setParseUrls(bool parseUrls);

    bool systemMessage() const;
    void setSystemMessage(bool systemMessage);

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
    void parseAttachment(const QJsonArray &attachments);

    // To be used in ID find: message ID
    bool operator==(const Message &other) const;

    // To be used in sorted insert: timestamp
    bool operator<(const Message &other) const;

    //Need for autotest as operator== is already defined
    bool isEqual(const Message &other) const;

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

    QString url() const;
    void setUrl(const QString &url);

    QString meta() const;
    void setMeta(const QString &meta);

    QString headers() const;
    void setHeaders(const QString &headers);

    QString parsedUrl() const;
    void setParsedUrl(const QString &parsedUrl);

    QString imageUrl() const;
    void setImageUrl(const QString &imageUrl);

    QString color() const;
    void setColor(const QString &color);

    QString alias() const;
    void setAlias(const QString &alias);

    QString systemMessageType() const;
    void setSystemMessageType(const QString &systemMessageType);

private:
    //Message Object Fields

    QVector<MessageAttachment> mAttachements;

    // _id
    QString mMessageId;


    // msg
    QString mText;

    // ts
    qint64 mTimeStamp = -1;

    // u
    QString mUsername;
    QString mUserId;

    // _updatedAt
    qint64 mUpdatedAt = -1;

    // editedAt
    qint64 mEditedAt = -1;

    // editedBy
    QString mEditedByUsername;
    QString mEditedByUserId;

    // urls
    QString mUrl;
    QString mMeta;
    QString mHeaders;
    QString mParsedUrl;

    // attachments
    QString mImageUrl;
    QString mColor;

    // alias
    QString mAlias;

    QString mSystemMessageType;

    // rid
    QString mRoomId;

    // avatar
    QString mAvatar;
    // groupable
    bool mGroupable = false;

    // parseUrls
    bool mParseUrls = false;

    bool mSystemMessage = false;
};
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const Message &t);
#endif // MESSAGE_H
