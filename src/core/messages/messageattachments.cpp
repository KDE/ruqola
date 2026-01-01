/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachments.h"
#include "messageutils.h"

#include "ruqola_message_memory_debug.h"
#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
MessageAttachments::MessageAttachments()
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

void MessageAttachments::setMessageAttachments(const QList<MessageAttachment> &attachments)
{
    mMessageAttachments = attachments;
}

QList<MessageAttachment> MessageAttachments::messageAttachments() const
{
    return mMessageAttachments;
}

void MessageAttachments::parseMessageAttachments(const QJsonArray &attachments, const QByteArray &messageId)
{
    mMessageAttachments.clear();
    // qDebug() << " void Message::parseAttachment(const QJsonObject &attachments)"<<attachments;
    for (int i = 0; i < attachments.size(); i++) {
        const QJsonObject attachment = attachments.at(i).toObject();
        MessageAttachment messageAttachement;
        messageAttachement.parseAttachment(attachment);
        messageAttachement.setAttachmentId(MessageUtils::generateUniqueId(messageId, i));
        if (messageAttachement.isValid()) {
            mMessageAttachments.append(messageAttachement);
        }
    }
}

bool MessageAttachments::operator==(const MessageAttachments &other) const
{
    return mMessageAttachments == other.mMessageAttachments;
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
    for (const auto &att : attachments.messageAttachments()) {
        array.append(MessageAttachment::serialize(att));
    }
    return array;
}

MessageAttachments *MessageAttachments::deserialize(const QJsonArray &attachmentsArray, const QByteArray &messageId)
{
    QList<MessageAttachment> attachmentList;
    for (int i = 0; i < attachmentsArray.count(); ++i) {
        const QJsonObject attachment = attachmentsArray.at(i).toObject();
        MessageAttachment att = MessageAttachment::deserialize(attachment);
        att.setAttachmentId(MessageUtils::generateUniqueId(messageId, i));
        if (att.isValid()) {
            attachmentList.append(std::move(att));
        }
    }
    auto final = new MessageAttachments;
    final->setMessageAttachments(attachmentList);
    return final;
}

bool MessageAttachments::isEmpty() const
{
    return mMessageAttachments.isEmpty();
}
