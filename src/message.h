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

    bool groupable() const;
    void setGroupable(bool groupable);

    bool parseUrls() const;
    void setParseUrls(bool parseUrls);

    bool systemMessage() const;
    void setSystemMessage(bool systemMessage);

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
    void parseAttachment(const QJsonObject &attachements);

    // To be used in ID find: message ID
    bool operator==(const Message &other) const;

    // To be used in sorted insert: timestamp
    bool operator<(const Message &other) const;

    //Need for autotest as operator== is already defined
    bool isEqual(const Message &other) const;

    //Message Object Fields

    QVector<MessageAttachment> mAttachements;

    // _id
    QString mMessageId;

    // rid
    QString mRoomId;

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

    // avatar
    QString mAvatar;
    QString mSystemMessageType;

private:
    // groupable
    bool mGroupable = false;

    // parseUrls
    bool mParseUrls = false;

    bool mSystemMessage = false;
};
#endif // MESSAGE_H
