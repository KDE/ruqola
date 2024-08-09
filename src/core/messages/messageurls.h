/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "block.h"
#include "libruqolacore_export.h"
#include <QList>
class LIBRUQOLACORE_EXPORT MessageUrls : public QSharedData
{
public:
    explicit MessageUrls(const MessageUrls &other);
    MessageUrls();
    ~MessageUrls();

    void setMessageUrls(const QList<Block> &reactions);
    [[nodiscard]] QList<Block> blocks() const;

    void parseMessageUrls(const QJsonObject &array);

    [[nodiscard]] bool operator==(const MessageUrls &other) const;

    [[nodiscard]] static QJsonObject serialize(const MessageUrls &reactions);
    [[nodiscard]] static MessageUrls *deserialize(const QJsonObject &o);

    [[nodiscard]] bool isEmpty() const;

private:
    QList<Block> mMessageUrls;
};
Q_DECLARE_METATYPE(MessageUrls)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const MessageUrls &t);
