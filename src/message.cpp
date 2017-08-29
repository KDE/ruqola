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

#include "message.h"

Message::Message()
{
}

void Message::parseMessage(const QJsonObject &o)
{
    const QString roomId = o.value(QStringLiteral("rid")).toString();

    //t ? I can't find it.
    const QString type = o.value(QStringLiteral("t")).toString();

    mMessageId = o.value(QStringLiteral("_id")).toString();
    mRoomId = roomId;
    mText = o.value(QStringLiteral("msg")).toString();
    mTimeStamp = (qint64)o.value(QStringLiteral("ts")).toObject().value(QStringLiteral("$date")).toDouble();
    mUsername = o.value(QStringLiteral("u")).toObject().value(QStringLiteral("username")).toString();
    mUserId = o.value(QStringLiteral("u")).toObject().value(QStringLiteral("_id")).toString();
    mUpdatedAt = o.value(QStringLiteral("_updatedAt")).toObject().value(QStringLiteral("$date")).toDouble();
    mEditedAt = o.value(QStringLiteral("editedAt")).toObject().value(QStringLiteral("$date")).toDouble();
    mEditedByUsername = o.value(QStringLiteral("editedBy")).toObject().value(QStringLiteral("username")).toString();
    mEditedByUserId = o.value(QStringLiteral("editedBy")).toObject().value(QStringLiteral("userID")).toString();
    mUrl = o.value(QStringLiteral("urls")).toObject().value(QStringLiteral("url")).toString();
    mMeta = o.value(QStringLiteral("urls")).toObject().value(QStringLiteral("meta")).toString();
    mHeaders = o.value(QStringLiteral("urls")).toObject().value(QStringLiteral("headers")).toString();
    mParsedUrl = o.value(QStringLiteral("urls")).toObject().value(QStringLiteral("parsedUrl")).toString();
    mImageUrl = o.value(QStringLiteral("attachments")).toObject().value(QStringLiteral("image_url")).toString();
    mColor = o.value(QStringLiteral("attachments")).toObject().value(QStringLiteral("color")).toString();
    mAlias = o.value(QStringLiteral("alias")).toString();
    mAvatar = o.value(QStringLiteral("avatar")).toString();
    mGroupable = o.value(QStringLiteral("groupable")).toBool();
    mParseUrls = o.value(QStringLiteral("parseUrls")).toBool();

    if (type.isEmpty()) {
        mSystemMessage = false;
    } else {
        mSystemMessage = true;
        mSystemMessageType = type;
    }
}

void Message::parseAttachment()
{
    //TODO
}

bool Message::operator==(const Message &other) const
{
    return other.mMessageId == mMessageId;
}

bool Message::operator<(const Message &other) const
{
    return mTimeStamp < other.mTimeStamp;
}
