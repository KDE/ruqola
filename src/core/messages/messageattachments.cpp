/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachments.h"

#include "ruqola_message_memory_debug.h"
#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
MessageAttachments::MessageAttachments()
    : QSharedData()
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " MessageAttachments created " << this;
}

MessageAttachments::MessageAttachments(const MessageAttachments &other)
    : QSharedData(other)
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " MessageAttachments(const MessageAttachments &other) created " << this;
    mMessageAttachments = other.messageAttachments();
}

MessageAttachments::~MessageAttachments()
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " MessageAttachments deleted " << this;
}

void MessageAttachments::setMessageAttachments(const QList<MessageAttachment> &blocks)
{
    mMessageAttachments = blocks;
}

QList<MessageAttachment> MessageAttachments::messageAttachments() const
{
    return mMessageAttachments;
}

void MessageAttachments::parseMessageAttachments(const QJsonObject &reactsr)
{
    mMessageAttachments.clear();
}

bool MessageAttachments::operator==(const MessageAttachments &other) const
{
    return mMessageAttachments == other.messageAttachments();
}

QDebug operator<<(QDebug d, const MessageAttachments &t)
{
    for (int i = 0; i < t.messageAttachments().count(); i++) {
        d.space() << t.messageAttachments().at(i) << "\n";
    }
    return d;
}

QJsonArray MessageAttachments::serialize(const MessageAttachments &attachments)
{
    QJsonArray array;
    for (const auto &attachmentInfo : attachments.messageAttachments()) {
        array.append(MessageAttachment::serialize(attachmentInfo));
    }
    return array;
}

MessageAttachments *MessageAttachments::deserialize(const QJsonArray &attachmentsArray, const QByteArray &messageId)
{
    QList<MessageAttachment> attachmentInfo;
    for (int i = 0; i < attachmentsArray.count(); ++i) {
        const QJsonObject attachment = attachmentsArray.at(i).toObject();
        MessageAttachment att = MessageAttachment::deserialize(attachment);
        att.setAttachmentId(MessageAttachments::generateUniqueId(messageId, i));
        if (att.isValid()) {
            attachmentInfo.append(std::move(att));
        }
    }
    auto final = new MessageAttachments;
    final->setMessageAttachments(attachmentInfo);
    return final;
}

QByteArray MessageAttachments::generateUniqueId(const QByteArray &messageId, int index)
{
    return messageId + QByteArray("_") + QByteArray::number(index);
}

bool MessageAttachments::isEmpty() const
{
    return mMessageAttachments.isEmpty();
}
