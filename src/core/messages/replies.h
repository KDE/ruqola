/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QList>
#include <QSharedData>
class LIBRUQOLACORE_EXPORT Replies : public QSharedData
{
public:
    explicit Replies(const Replies &other);
    Replies();
    ~Replies();

    void parseReplies(const QJsonArray &array);

    [[nodiscard]] bool operator==(const Replies &other) const;

    [[nodiscard]] static QJsonArray serialize(const Replies &channels);
    [[nodiscard]] static Replies *deserialize(const QJsonArray &o);

    [[nodiscard]] bool isEmpty() const;

    void setReplies(const QList<QByteArray> &replies);
    [[nodiscard]] QList<QByteArray> replies() const;

private:
    QList<QByteArray> mReplies;
};
Q_DECLARE_METATYPE(Replies)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Replies &t);
