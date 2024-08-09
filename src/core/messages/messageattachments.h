/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "block.h"
#include "libruqolacore_export.h"
#include <QList>
class LIBRUQOLACORE_EXPORT MessageAttachments : public QSharedData
{
public:
    explicit MessageAttachments(const MessageAttachments &other);
    MessageAttachments();
    ~MessageAttachments();

    void setMessageAttachments(const QList<Block> &reactions);
    [[nodiscard]] QList<Block> blocks() const;

    void parseMessageAttachments(const QJsonObject &array);

    [[nodiscard]] bool operator==(const MessageAttachments &other) const;

    [[nodiscard]] static QJsonObject serialize(const MessageAttachments &reactions);
    [[nodiscard]] static MessageAttachments *deserialize(const QJsonObject &o);

    [[nodiscard]] bool isEmpty() const;

private:
    QList<Block> mMessageAttachments;
};
Q_DECLARE_METATYPE(MessageAttachments)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const MessageAttachments &t);
