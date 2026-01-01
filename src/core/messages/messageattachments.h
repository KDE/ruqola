/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "messageattachment.h"
#include <QList>
#include <QSharedData>
class LIBRUQOLACORE_EXPORT MessageAttachments : public QSharedData
{
public:
    explicit MessageAttachments(const MessageAttachments &other);
    MessageAttachments();
    ~MessageAttachments();

    void setMessageAttachments(const QList<MessageAttachment> &attachments);
    [[nodiscard]] QList<MessageAttachment> messageAttachments() const;

    void parseMessageAttachments(const QJsonArray &attachments, const QByteArray &messageId);

    [[nodiscard]] bool operator==(const MessageAttachments &other) const;

    [[nodiscard]] static QJsonArray serialize(const MessageAttachments &attachments);
    [[nodiscard]] static MessageAttachments *deserialize(const QJsonArray &o, const QByteArray &messageId);

    [[nodiscard]] bool isEmpty() const;

private:
    QList<MessageAttachment> mMessageAttachments;
};
QT_DECL_METATYPE_EXTERN_TAGGED(MessageAttachments, Ruqola_MessageAttachments, LIBRUQOLACORE_EXPORT)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const MessageAttachments &t);
