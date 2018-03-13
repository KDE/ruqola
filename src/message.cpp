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

#include "message.h"
#include "utils.h"
#include "ruqola_debug.h"
#include <QDebug>

#include <QJsonArray>
#include <QJsonDocument>

Message::Message()
{
}

void Message::parseMessage(const QJsonObject &o)
{
    const QString roomId = o.value(QLatin1String("rid")).toString();

    //t ? I can't find it.
    const QString type = o.value(QLatin1String("t")).toString();

    mMessageId = o.value(QLatin1String("_id")).toString();
    mRoomId = roomId;
    mText = o.value(QLatin1String("msg")).toString();
    mTimeStamp = Utils::parseDate(QLatin1String("ts"), o);
    mUsername = o.value(QLatin1String("u")).toObject().value(QLatin1String("username")).toString();
    mUserId = o.value(QLatin1String("u")).toObject().value(QLatin1String("_id")).toString();
    mUpdatedAt = Utils::parseDate(QLatin1String("_updatedAt"), o);
    mEditedAt = Utils::parseDate(QLatin1String("editedAt"), o);
    mEditedByUsername = o.value(QLatin1String("editedBy")).toObject().value(QLatin1String("username")).toString();
    mEditedByUserId = o.value(QLatin1String("editedBy")).toObject().value(QLatin1String("_id")).toString();
    mAlias = o.value(QLatin1String("alias")).toString();
    mAvatar = o.value(QLatin1String("avatar")).toString();
    mGroupable = o.value(QLatin1String("groupable")).toBool();
    mParseUrls = o.value(QLatin1String("parseUrls")).toBool();
    if (o.contains(QLatin1String("starred"))) {
        mStarred = !o.value(QStringLiteral("starred")).toArray().isEmpty();
    } else {
        mStarred = false;
    }

    mMessageType = Message::MessageType::NormalText;
    if (!type.isEmpty()) {
        mSystemMessageType = type;
        mMessageType = System;
    }
    parseMentions(o.value(QLatin1String("mentions")).toArray());

    parseAttachment(o.value(QLatin1String("attachments")).toArray());
    parseUrls(o.value(QLatin1String("urls")).toArray());
    parseReactions(o.value(QLatin1String("reactions")).toObject());
}

void Message::parseReactions(const QJsonObject &reacts)
{
    if (!reacts.isEmpty()) {
        mReactions.parseReactions(reacts);
    }
}


void Message::parseMentions(const QJsonArray &mentions)
{
    mMentions.clear();
    for (int i = 0; i < mentions.size(); i++) {
        const QJsonObject mention = mentions.at(i).toObject();
        mMentions.insert(mention.value(QLatin1String("username")).toString(), mention.value(QLatin1String("_id")).toString());
    }
}

void Message::parseUrls(const QJsonArray &urls)
{
    mUrls.clear();
    if (!urls.isEmpty()) {
        qCDebug(RUQOLA_LOG) << " void Message::urls(const QJsonObject &attachements)"<<urls;
        for (int i = 0; i < urls.size(); i++) {
            const QJsonObject url = urls.at(i).toObject();
            MessageUrl messageUrl;
            const QJsonValue urlStr = url.value(QLatin1String("url"));
            if (!urlStr.isUndefined()) {
                messageUrl.setUrl(urlStr.toString());
            }
            const QJsonObject meta = url.value(QLatin1String("meta")).toObject();
            if (!meta.isEmpty()) {
                const QJsonValue pageTitleStr = meta.value(QLatin1String("pageTitle"));
                if (!pageTitleStr.isUndefined()) {
                    messageUrl.setPageTitle(pageTitleStr.toString());
                }
                const QJsonValue descriptionStr = meta.value(QLatin1String("description"));
                if (!descriptionStr.isUndefined()) {
                    messageUrl.setDescription(descriptionStr.toString());
                }
            }
            //TODO add more

            if (!messageUrl.isEmpty()) {
                mUrls.append(messageUrl);
            }
        }
    }
}

Reactions Message::reactions() const
{
    return mReactions;
}

void Message::setReactions(const Reactions &reactions)
{
    mReactions = reactions;
}

bool Message::starred() const
{
    return mStarred;
}

void Message::setStarred(bool starred)
{
    mStarred = starred;
}

QMap<QString, QString> Message::mentions() const
{
    return mMentions;
}

void Message::setMentions(const QMap<QString, QString> &mentions)
{
    mMentions = mentions;
}

void Message::parseAttachment(const QJsonArray &attachments)
{
    mAttachements.clear();
    if (!attachments.isEmpty()) {
        qCDebug(RUQOLA_LOG) << " void Message::parseAttachment(const QJsonObject &attachements)"<<attachments;
        for (int i = 0; i < attachments.size(); i++) {
            const QJsonObject attachment = attachments.at(i).toObject();
            MessageAttachment messageAttachement;
            const QJsonValue description = attachment.value(QLatin1String("description"));
            if (!description.isUndefined()) {
                messageAttachement.setDescription(description.toString());
            }
            const QJsonValue title = attachment.value(QLatin1String("title"));
            if (!title.isUndefined()) {
                messageAttachement.setTitle(title.toString());
            }

            if (attachment.contains(QLatin1String("audio_url"))) {
                messageAttachement.setLink(attachment.value(QLatin1String("audio_url")).toString());
                mMessageType = Message::MessageType::Audio;
            } else if (attachment.contains(QLatin1String("video_url"))) {
                messageAttachement.setLink(attachment.value(QLatin1String("video_url")).toString());
                mMessageType = Message::MessageType::Video;
            } else if (attachment.contains(QLatin1String("image_url"))) {
                messageAttachement.setLink(attachment.value(QLatin1String("image_url")).toString());
                mMessageType = Message::MessageType::Image;
            } else if (attachment.contains(QLatin1String("title_link"))) { //Last as an image_url can have a title_link
                messageAttachement.setLink(attachment.value(QLatin1String("title_link")).toString());
                mMessageType = Message::MessageType::File;
            }
            //Add image dimension
            const QJsonValue imageDimensions = attachment.value(QLatin1String("image_dimensions"));
            if (!imageDimensions.isUndefined()) {
                const QJsonObject imageDimensionsParams = imageDimensions.toObject();

                messageAttachement.setImageHeight(imageDimensionsParams.value(QLatin1String("height")).toInt());
                messageAttachement.setImageWidth(imageDimensionsParams.value(QLatin1String("width")).toInt());
                //TODO validate image size
            }

            messageAttachement.setAuthorName(attachment.value(QLatin1String("author_name")).toString());
            //Color
            const QJsonValue color = attachment.value(QLatin1String("color"));
            if (!color.isUndefined()) {
                messageAttachement.setColor(color.toString());
            }

            if (!messageAttachement.isEmpty()) {
                mAttachements.append(messageAttachement);
            }
        }
    }
}

bool Message::operator==(const Message &other) const
{
    return (mMessageId == other.messageId())
           && (mRoomId == other.roomId())
           && (mText == other.text())
           && (mTimeStamp == other.timeStamp())
           && (mUsername == other.username())
           && (mUserId == other.userId())
           && (mUpdatedAt == other.updatedAt())
           && (mEditedAt == other.editedAt())
           && (mEditedByUsername == other.editedByUsername())
           && (mEditedByUserId == other.editedByUserId())
           && (mAlias == other.alias())
           && (mAvatar == other.avatar())
           && (mSystemMessageType == other.systemMessageType())
           && (mGroupable == other.groupable())
           && (mParseUrls == other.parseUrls())
           && (mUrls == other.urls())
           && (mAttachements == other.attachements())
           && (mMentions == other.mentions())
           && (mStarred == other.starred());
}

Message &Message::operator=(const Message &other)
{
    setMessageId(other.messageId());
    setRoomId(other.roomId());
    setText(other.text());
    setTimeStamp(other.timeStamp());
    setUsername(other.username());
    setUserId(other.userId());
    setUpdatedAt(other.updatedAt());
    setEditedAt(other.editedAt());
    setEditedByUsername(other.editedByUsername());
    setEditedByUserId(other.editedByUserId());
    setAlias(other.alias());
    setAvatar(other.avatar());
    setSystemMessageType(other.systemMessageType());
    setGroupable(other.groupable());
    setParseUrls(other.parseUrls());
    setUrls(other.urls());
    setAttachements(other.attachements());
    setMentions(other.mentions());
    setMessageType(other.messageType());
    setStarred(other.starred());
    return *this;
}

bool Message::operator<(const Message &other) const
{
    return mTimeStamp < other.mTimeStamp;
}

QString Message::systemMessageType() const
{
    return mSystemMessageType;
}

void Message::setSystemMessageType(const QString &systemMessageType)
{
    mSystemMessageType = systemMessageType;
}

Message::MessageType Message::messageType() const
{
    return mMessageType;
}

void Message::setMessageType(const MessageType &messageType)
{
    mMessageType = messageType;
}

QVector<MessageAttachment> Message::attachements() const
{
    return mAttachements;
}

void Message::setAttachements(const QVector<MessageAttachment> &attachements)
{
    mAttachements = attachements;
}

QVector<MessageUrl> Message::urls() const
{
    return mUrls;
}

void Message::setUrls(const QVector<MessageUrl> &urls)
{
    mUrls = urls;
}

QString Message::alias() const
{
    return mAlias;
}

void Message::setAlias(const QString &alias)
{
    mAlias = alias;
}

QString Message::editedByUserId() const
{
    return mEditedByUserId;
}

void Message::setEditedByUserId(const QString &editedByUserId)
{
    mEditedByUserId = editedByUserId;
}

QString Message::editedByUsername() const
{
    return mEditedByUsername;
}

void Message::setEditedByUsername(const QString &editedByUsername)
{
    mEditedByUsername = editedByUsername;
}

qint64 Message::editedAt() const
{
    return mEditedAt;
}

void Message::setEditedAt(const qint64 &editedAt)
{
    mEditedAt = editedAt;
}

qint64 Message::updatedAt() const
{
    return mUpdatedAt;
}

void Message::setUpdatedAt(const qint64 &updatedAt)
{
    mUpdatedAt = updatedAt;
}

QString Message::userId() const
{
    return mUserId;
}

void Message::setUserId(const QString &userId)
{
    mUserId = userId;
}

QString Message::username() const
{
    return mUsername;
}

void Message::setUsername(const QString &username)
{
    mUsername = username;
}

qint64 Message::timeStamp() const
{
    return mTimeStamp;
}

void Message::setTimeStamp(const qint64 &timeStamp)
{
    mTimeStamp = timeStamp;
}

QString Message::text() const
{
    return mText;
}

void Message::setText(const QString &text)
{
    mText = text;
}

QString Message::messageId() const
{
    return mMessageId;
}

void Message::setMessageId(const QString &messageId)
{
    mMessageId = messageId;
}

QString Message::roomId() const
{
    return mRoomId;
}

void Message::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QString Message::avatar() const
{
    return mAvatar;
}

void Message::setAvatar(const QString &avatar)
{
    mAvatar = avatar;
}

bool Message::parseUrls() const
{
    return mParseUrls;
}

void Message::setParseUrls(bool parseUrls)
{
    mParseUrls = parseUrls;
}

bool Message::groupable() const
{
    return mGroupable;
}

void Message::setGroupable(bool groupable)
{
    mGroupable = groupable;
}

Message Message::fromJSon(const QJsonObject &o)
{
    Message message;
    //Add reactions ! "reactions"

    message.mMessageId = o[QStringLiteral("messageID")].toString();
    message.mRoomId = o[QStringLiteral("roomID")].toString();
    message.mText = o[QStringLiteral("message")].toString();
    message.mTimeStamp = (qint64)o[QStringLiteral("timestamp")].toDouble();
    message.mUsername = o[QStringLiteral("username")].toString();
    message.mUserId = o[QStringLiteral("userID")].toString();
    message.mUpdatedAt = (qint64)o[QStringLiteral("updatedAt")].toDouble();
    message.mEditedAt = (qint64)o[QStringLiteral("editedAt")].toDouble();
    message.mEditedByUsername = o[QStringLiteral("editedByUsername")].toString();
    message.mEditedByUserId = o[QStringLiteral("editedByUserID")].toString();
    message.mAlias = o[QStringLiteral("alias")].toString();
    message.mAvatar = o[QStringLiteral("avatar")].toString();
    message.mGroupable = o[QStringLiteral("groupable")].toBool();
    message.mParseUrls = o[QStringLiteral("parseUrls")].toBool();
    message.mStarred = o[QStringLiteral("starred")].toBool();

    message.mSystemMessageType = o[QStringLiteral("type")].toString();
    message.mMessageType = o[QStringLiteral("messageType")].toVariant().value<MessageType>();
    const QJsonArray attachmentsArray = o.value(QLatin1String("attachments")).toArray();
    for (int i = 0; i < attachmentsArray.count(); ++i) {
        const QJsonObject attachment = attachmentsArray.at(i).toObject();
        const MessageAttachment att = MessageAttachment::fromJSon(attachment);
        if (!att.isEmpty()) {
            message.mAttachements.append(att);
        }
    }
    const QJsonArray urlsArray = o.value(QLatin1String("urls")).toArray();
    for (int i = 0; i < urlsArray.count(); ++i) {
        const QJsonObject urlObj = urlsArray.at(i).toObject();
        const MessageUrl url = MessageUrl::fromJSon(urlObj);
        if (!url.isEmpty()) {
            message.mUrls.append(url);
        }
    }
    const QJsonArray mentionsArray = o.value(QLatin1String("mentions")).toArray();
    for (int i = 0; i < mentionsArray.count(); ++i) {
        const QJsonObject mention = mentionsArray.at(i).toObject();
        qCDebug(RUQOLA_LOG) << " mention"<<mention;
//        const MessageAttachment att = MessageAttachment::fromJSon(attachment);
//        if (!att.isEmpty()) {
//            message.mAttachements.append(att);
//        }
    }

    //TODO reactions

    return message;
}

QByteArray Message::serialize(const Message &message, bool toBinary)
{
    QJsonDocument d;
    QJsonObject o;

    o[QStringLiteral("messageID")] = message.mMessageId;
    o[QStringLiteral("roomID")] = message.mRoomId;
    o[QStringLiteral("message")] = message.mText;
    o[QStringLiteral("timestamp")] = message.mTimeStamp;
    o[QStringLiteral("username")] = message.mUsername;
    o[QStringLiteral("userID")] = message.mUserId;
    o[QStringLiteral("updatedAt")] = message.mUpdatedAt;
    o[QStringLiteral("editedAt")] = message.mEditedAt;
    o[QStringLiteral("editedByUsername")] = message.mEditedByUsername;
    o[QStringLiteral("editedByUserID")] = message.mEditedByUserId;
    o[QStringLiteral("alias")] = message.mAlias;
    o[QStringLiteral("avatar")] = message.mAvatar;
    o[QStringLiteral("groupable")] = message.mGroupable;
    o[QStringLiteral("parseUrls")] = message.mParseUrls;
    o[QStringLiteral("starred")] = message.mStarred;

    o[QStringLiteral("type")] = message.mSystemMessageType;
    o[QStringLiteral("messageType")] = QJsonValue::fromVariant(QVariant::fromValue<Message::MessageType>(message.mMessageType));
    //TODO add mentions
    //Attachments
    if (!message.mAttachements.isEmpty()) {
        QJsonArray array;
        const int nbAttachment{
            message.mAttachements.count()
        };
        for (int i = 0; i < nbAttachment; ++i) {
            array.append(MessageAttachment::serialize(message.mAttachements.at(i)));
        }
        o[QStringLiteral("attachments")] = array;
    }
    QMapIterator<QString, QString> i(message.mentions());
    while (i.hasNext()) {
        i.next();
        //TODO
    }
    //Urls
    if (!message.mUrls.isEmpty()) {
        QJsonArray array;
        const int nbUrls{
            message.mUrls.count()
        };
        for (int i = 0; i < nbUrls; ++i) {
            array.append(MessageUrl::serialize(message.mUrls.at(i)));
        }
        o[QStringLiteral("urls")] = array;
    }
    //TODO reactions

    d.setObject(o);
    if (toBinary) {
        return d.toBinaryData();
    }
    return d.toJson(QJsonDocument::Indented);
}

QDebug operator <<(QDebug d, const Message &t)
{
    d << "mMessageId : " << t.messageId();
    d << "mText: " << t.text();
    d << "mTimeStamp: " << t.timeStamp();
    d << "mUsername: " << t.username();
    d << "mUserId: " << t.userId();
    d << "mUpdatedAt: " << t.updatedAt();
    d << "mEditedAt: " << t.editedAt();
    d << "mEditedByUsername: " << t.editedByUsername();
    d << "mEditedByUserId: " << t.editedByUserId();
    d << "mAlias: " << t.alias();
    d << "mSystemMessageType: " << t.systemMessageType();
    d << "mRoomId: " << t.roomId();
    d << "mAvatar: " << t.avatar();
    d << "mGroupable: " << t.groupable();
    d << "mParseUrls: " << t.parseUrls();
    d << "mStarred: " << t.starred();
    for (int i = 0, total = t.attachements().count(); i < total; ++i) {
        d << "Attachment :" << t.attachements().at(i);
    }
    for (int i = 0, total = t.urls().count(); i < total; ++i) {
        d << "Urls :" << t.urls().at(i);
    }
    d << "Mentions :" << t.mentions();
    d << "mMessageType: " << t.messageType();
    //TODO reactions
    return d;
}
