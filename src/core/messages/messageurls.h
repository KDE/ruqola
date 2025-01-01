/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "messageurl.h"
#include <QList>
class LIBRUQOLACORE_EXPORT MessageUrls : public QSharedData
{
public:
    explicit MessageUrls(const MessageUrls &other);
    MessageUrls();
    ~MessageUrls();

    void setMessageUrls(const QList<MessageUrl> &messageUrls);
    [[nodiscard]] QList<MessageUrl> messageUrls() const;

    void parseMessageUrls(const QJsonArray &urls, const QByteArray &messageId);

    [[nodiscard]] bool operator==(const MessageUrls &other) const;

    [[nodiscard]] static QJsonArray serialize(const MessageUrls &urls);
    [[nodiscard]] static MessageUrls *deserialize(const QJsonArray &o, const QByteArray &messageId);

    [[nodiscard]] bool isEmpty() const;

private:
    QList<MessageUrl> mMessageUrls;
};
Q_DECLARE_METATYPE(MessageUrls)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const MessageUrls &t);
